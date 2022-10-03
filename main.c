/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/10/03 18:12:27 by tkempf-e         ###   ########.fr       */
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
	long int		number_of_philo;
	long int		initial_time_to_die;
	long int		initial_time_to_eat;
	long int		initial_time_to_sleep;
}	t_data;

typedef struct s_philo_data
{
	int				name;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long long		time_left;//not used
	pthread_mutex_t	fork;
	pthread_mutex_t	*right_fork;
	int				is_fork_locked;
	int				*is_right_fork_locked;
	long long int	time_now;
	long int		times_each_philo_must_eat;//not used
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
	return (0);
}

int	ft_error(void)
{
	printf("Arguments Error\n");
	return (-1);
}

void	ft_memset(t_philo_data *philo, int nbr, long int size)
{
	int		i;

	i = 0;
	while (i++ < size)
		philo->times_each_philo_must_eat = (int long) nbr;
}

void	init_fork(t_data *data, t_philo_data *philo, t_philo_data *philo2)
{
	philo->right_fork = &philo2->fork;
	philo->is_fork_locked = 0;
	philo->is_right_fork_locked = &philo2->is_fork_locked;
	pthread_mutex_init(&philo->fork, NULL);
}

long long int	gettime(void)
{
	struct timeval	t;
	long long int	time;

	gettimeofday(&t, NULL);
	time = t.tv_sec * 1000;
	time += t.tv_usec / 1000;
	return (time);
}

void	philo_init(t_data *data, t_philo_data *philo, int argc, char **argv)
{
	static int	i = 0;

	philo->name = i + 1;
	philo->time_to_die = data->initial_time_to_die * 1000;
	philo->time_to_eat = data->initial_time_to_eat * 1000;
	philo->time_to_sleep = data->initial_time_to_sleep * 1000;
	philo->time_now = gettime() * 1000;
	if (argc == 6)
		philo->times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		philo->times_each_philo_must_eat = -1;
	i++;
}

long int	time_diff(struct timeval time_now)
{
	long int		diff;
	struct timeval	end;

	gettimeofday(&end, NULL);
	diff = ((end.tv_sec * 1000) + (end.tv_usec / 1000)) - ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000));
	return (diff);
}
//a faire
void	take_fork(t_philo_data *philo)
{
	if (philo->is_fork_locked == 0)
	{
		pthread_mutex_lock(&philo->fork);
		philo->is_fork_locked = 1;
	}
	if (philo->is_right_fork_locked == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		*philo->is_right_fork_locked = 1;
	}
}

//mange un certain temps et doit garder ses fourchettes pendant ce temps
//reset la faim
void	eating(t_philo_data *philo)
{
	printf("%lld    %d is eating\n", gettime() - philo->time_now, philo->name);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleeping(t_philo_data *philo)
{
	printf("%lld   %d is sleeping\n", gettime() - philo->time_now, philo->name);
	usleep(philo->time_to_sleep);
}

void	thinking(t_philo_data *philo)
{
	printf("%lld   %d is thinking\n", gettime() - philo->time_now, philo->name);
	usleep(philo->time_to_sleep);
}

// void	died(t_philo_data *philo, t_data *data)
// {
// 	printf("%ld   %d died\n", time_diff(&data->time_now, &data->end), philo->name);
// }

void	philosopher(t_philo_data *philo)
{
	philo->time_now = gettime();
	while (1)
	{
		// take_fork(philo);
		// eating(philo);
		sleeping(philo);
		thinking(philo);
	}
}

// take in account time to die/eat/sleep
// number_of_philos		time_to_die 	time_to_eat		time_to_sleep	[times_each_philo_must_eat]

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

//faire fonctionnement fourchettes
int	main(int argc, char *argv[])
{
	t_data				data;
	t_philo_data		philo[250];
	int					i;

	if (arguments_checker(argc, argv, &data) == -1)
		return (ft_error());
	i = 0;
	while (i++ < data.number_of_philo)
		philo_init(&data, &philo[i], argc, argv);
	init_fork(&data, &philo[0], &philo[data.number_of_philo - 1]);
	i = 1;
	while (i++ < data.number_of_philo)
		init_fork(&data, &philo[i], &philo[i - 1]);
	i = 0;
	while (i++ < data.number_of_philo)
		pthread_create(&philo[i].thread, NULL, (void *)philosopher, &philo[i]);
	i = 0;
	while (i++ < data.number_of_philo)
		pthread_join(philo[i].thread, NULL);
	return (0);
}

/*
	Threads : nouveaux processus executant une fonction.
pthread_join est un point ou les threads s'attendent.
	Mutex : mutex_lock et mutex_unlock placent une zone qui ne peut etre lu que par un thread a la fois.
*/