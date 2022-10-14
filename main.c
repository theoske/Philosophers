/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/10/14 16:43:54 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

// eat -> sleep -> think		#SigmaPhilosopherGrindset
// 1 fork per philo

int	ft_finder(const char *nptr)
{
	int		i;

	i = 0;
	while (nptr[i] == ' ' || nptr[i] == '\t' || nptr[i] == '\n'
		|| nptr[i] == '\v' || nptr[i] == '\f' || nptr[i] == '\r')
		i++;
	if (nptr[i] == '+')
		i++;
	return (i);
}

int	ft_atoi(const char *nptr)
{
	int			i;
	int			j;
	int			neg;
	long int	nb;

	neg = 1;
	i = ft_finder(nptr);
	nb = 0;
	if (nptr[i] == '-' && nptr[i - 1] != '+')
	{
		neg = -1;
		i++;
	}
	j = i;
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		nb *= 10;
		nb += (nptr[i] - 48);
		i++;
	}
	if (i - j > 10 && neg == 1)
		return (-1);
	if (i - j > 10)
		return (0);
	return (nb * neg);
}

typedef struct s_data
{
	long long int	number_of_philo;
	long long int	initial_time_to_die;
	long long int	initial_time_to_eat;
	long long int	initial_time_to_sleep;
	int				is_dead;
	pthread_mutex_t	talk;// a init

}	t_data;

typedef struct s_philo_data
{
	int				name;
	long long int	last_meal;
	long long int	time_to_die;
	long long int	time_to_eat;
	long long int	time_to_sleep;
	pthread_mutex_t	fork;
	pthread_mutex_t	*right_fork;
	long long int	time_now;
	int				times_each_philo_must_eat;
	int				time_eaten;
	t_data			*data;
	pthread_t		thread;
}	t_philo_data;

int	number_checker(char *argv[])
{
	int		i;
	int		j;

	i = 1;
	while (argv && argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

/*
	Checking if arguments are numbers.
	Checking if there is a valid number of arguments (4 or 5).
	Converting them from char* to int.
*/
int	arguments_checker(int argc, char *argv[], t_data *data)
{
	if (number_checker(argv) == -1 || argc < 5 || argc > 6)
		return (-1);
	data->number_of_philo = ft_atoi(argv[1]);
	data->initial_time_to_die = ft_atoi(argv[2]);
	data->initial_time_to_eat = ft_atoi(argv[3]);
	data->initial_time_to_sleep = ft_atoi(argv[4]);
	data->is_dead = 0;
	pthread_mutex_init(&data->talk, NULL);
	return (0);
}

int	ft_error(void)
{
	printf("Arguments Error\n");
	return (-1);
}

long long int	gettime(void)
{
	struct timeval	t;
	long long int	time;

	gettimeofday(&t, NULL);
	time = (t.tv_sec) * 1000 + (t.tv_usec) / 1000;
	return (time);
}

void	philo_init(t_data *data, t_philo_data *philo, int argc, char **argv)
{
	static int	i = 0;

	philo->name = i + 1;
	philo->time_to_die = data->initial_time_to_die;
	philo->time_to_eat = data->initial_time_to_eat * 1000;
	philo->time_to_sleep = data->initial_time_to_sleep * 1000;
	if (argc == 6)
		philo->times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		philo->times_each_philo_must_eat = -1;
	i++;
	philo->data = data;
	philo->time_eaten = 0;
	philo->last_meal = gettime();
}

/*
0 : take fork
1 : eat
2 : sleep
3 : think
4 : died
*/
void	talking(t_philo_data *philo, int option)
{
	pthread_mutex_lock(&philo->data->talk);
	if (option == 0 && philo->data->is_dead == 0)
		printf("%lld	%d has taken a fork.\n", gettime() - philo->time_now, philo->name);
	else if (option == 1 && philo->data->is_dead == 0)
		printf("%lld	%d is eating.\n", gettime() - philo->time_now, philo->name);
	else if (option == 2 && philo->data->is_dead == 0)
		printf("%lld	%d is sleeping.\n", gettime() - philo->time_now, philo->name);
	else if (option == 3 && philo->data->is_dead == 0)
		printf("%lld	%d is thinking.\n", gettime() - philo->time_now, philo->name);
	else if (option == 4 && philo->data->is_dead != 0)
	{
		printf("%lld	%d died.\n", gettime() - philo->time_now, philo->name);
		philo->data->is_dead = 1;
	}
	pthread_mutex_unlock(&philo->data->talk);
}

int	died(t_philo_data *philo)//rentre dedans avec retard
{
	long long int	time_no_eat;

	if (philo->data->is_dead != 0)
		return (-1);
	time_no_eat = gettime() - philo->last_meal;
	if (time_no_eat > philo->time_to_die)
	{
		pthread_mutex_lock(&philo->data->talk);
		philo->data->is_dead--;
		if (philo->data->is_dead == -1)
			printf("%lld	%d died.\n", gettime() - philo->time_now, philo->name);
		pthread_mutex_unlock(&philo->data->talk);
		return (-1);
	}
	return (0);
}

void	take_fork(t_philo_data *philo)
{
	if (philo->name % 2 == 1)
	{
		if (philo->data->initial_time_to_die < philo->data->initial_time_to_eat)
			{
				usleep(philo->time_to_die * 1000);
				died(philo);
			}
		else
			usleep(philo->time_to_eat * 9 / 10);
	}
	if (died(philo) == 0)
	{
		pthread_mutex_lock(&philo->fork);
		talking(philo, 0);
		pthread_mutex_lock(philo->right_fork);
		talking(philo, 0);
	}
}

void	eating(t_philo_data *philo)
{
	take_fork(philo);
	if (died(philo) == 0)
		talking(philo, 1);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(philo->right_fork);
	philo->last_meal = gettime();
	philo->time_eaten++;
}

void	sleeping(t_philo_data *philo)
{
	talking(philo, 2);
	usleep(philo->time_to_sleep);
}

void	thinking(t_philo_data *philo)
{
	talking(philo, 3);
}

void	philosopher(t_philo_data *philo)
{
	philo->time_now = gettime();
	while (philo->time_eaten != philo->times_each_philo_must_eat)
	{
		eating(philo);
		if (died(philo) == 0)
			sleeping(philo);
		if (died(philo) == 0)
			thinking(philo);
		if (died(philo) != 0)
			break;
	}
}

// take in account time to die/eat/sleep
// number_of_philos		data->initial_time_to_die 	data->initial_time_to_eat		data->initial_time_to_sleep	[times_each_philo_must_eat]

/*
	chaque fourchette doit etre representee par une valeur dans un tableau.
	avec un 1 si elle est disponible ou 0 si elle est prise.
	
	while (TRUE)
	{
		wait(fork[i])
		wait(fork[(i + 1) % nbre de philo])
		
		*eat*
		signal(fork[i])
		signal(fork[(i + 1) % nbre de philo])
		*sleep*
		*think*
	}
	personne ne mange en meme temps mais deadlock qui bloque le programme

	Solutions :
				- /!\ un philosophe commence sans fourchette  /!\ tester celle là
				- les philosophes ne peuvent prendre une fourchette que si elle est libre
				- philos paires prennent fourchette de gauche et impairs celle de droite
*/

//premier philo arrive pas a prendre fourchette dernier philo
int	main(int argc, char *argv[])
{
	t_data				data;
	t_philo_data		philo[250];
	int					i;

	if (arguments_checker(argc, argv, &data) == -1)
		return (ft_error());
	i = 0;
	while (i < data.number_of_philo)
	{
		philo_init(&data, &philo[i], argc, argv);
		i++;
	}
	i = 0;
	while (i < data.number_of_philo)
	{
		pthread_mutex_init(&philo[i].fork, NULL);
		i++;
	}
	i = 1;
	while (i < data.number_of_philo)
	{
		philo[i].right_fork = &philo[i - 1].fork;
		i++;
	}
	philo[0].right_fork = &philo[data.number_of_philo - 1].fork;
	i = 0;
	while (i < data.number_of_philo)
	{
		pthread_create(&philo[i].thread, NULL, (void *)philosopher, &philo[i]);
		i++;
	}
	i = 0;
	while (i < data.number_of_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	return (0);
}

/*
	Threads : nouveaux processus executant une fonction.
pthread_join est un point ou les threads s'attendent.
	Mutex : mutex_lock et mutex_unlock placent une zone qui ne peut etre lu que par un thread a la fois.
*/
