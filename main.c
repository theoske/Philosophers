/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/08/29 18:15:33 by tkempf-e         ###   ########.fr       */
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
	long long		time_left;
	int				right_fork;
	int				left_fork;
	struct timeval	start;
	long int		times_each_philo_must_eat;
	pthread_t		thread;
	pthread_mutex_t	fork;
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

void	ft_memset(t_philo_data *philo_data, int nbr, long int size)
{
	int		i;

	i = 0;
	while (i++ < size)
		philo_data->times_each_philo_must_eat = (int long) nbr;
}

void	philo_init(t_data *data, t_philo_data *philo_data, int argc, char **argv)
{
	int				i;
	// t_philo_data	*philo = malloc(sizeof(t_philo_data));

	i = 0;
	philo_data = malloc(sizeof(t_philo_data) * data->number_of_philo);
	while (i < data->number_of_philo)
	{
		philo_data[i].name = i + 1;
		philo_data[i].time_to_die = data->initial_time_to_die;
		philo_data[i].time_to_eat = data->initial_time_to_eat;
		philo_data[i].time_to_sleep = data->initial_time_to_sleep;
		gettimeofday(&philo_data[i].start, NULL);
		if (argc == 6)
			philo_data[i].times_each_philo_must_eat = ft_atoi(argv[5]);
		else
			philo_data[i].times_each_philo_must_eat = -1;
		i++;
	}
}

long int	time_diff(struct timeval start)
{
	long int		diff;
	struct timeval	end;

	gettimeofday(&end, NULL);
	diff = ((end.tv_sec * 1000) + (end.tv_usec / 1000)) - ((start.tv_sec * 1000) + (start.tv_usec / 1000));
	return (diff);
}

void	take_fork(t_philo_data *philo_data, t_data *data, int i)
{
	philo_data->right_fork++;
	printf("%ld   %d has taken a fork\n", time_diff(philo_data->start), philo_data[i].name);
}

//mange un certain temps et doit garder ses fourchettes pendant ce temps
//reset la faim
void	eat(t_philo_data **philo_data, t_data *data)
{
	int		i;

	i = 0;
	while (i < data->number_of_philo)
	{
					
		i++;
	}
}

// void	sleeping(t_philo_data *philo_data, t_data *data)
// {
// 	printf("%ld   %d is sleeping\n", time_diff(&data->start, &data->end), philo_data->name);
// }

// void	think(t_philo_data *philo_data, t_data *data)
// {
// 	printf("%ld   %d is thinking\n", time_diff(&data->start, &data->end), philo_data->name);
// }

// void	died(t_philo_data *philo_data, t_data *data)
// {
// 	printf("%ld   %d died\n", time_diff(&data->start, &data->end), philo_data->name);
// }

// fin quand 1 philo meurt ou quand tous les philo ont suffisament mangé
void	philosopher(t_philo_data *philo_data, t_data *data)
{
	int			i;
	pthread_t	*thread;

	i = 0;
	while (1)
	{
		eat(philo_data, data);
		printf("a\n");
		i++;
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
int	main(int argc, char *argv[])
{
	t_data				data;
	t_philo_data		*philo_data;

	if (arguments_checker(argc, argv, &data) == -1)
		return (ft_error());
	philo_init(&data, philo_data, argc, argv);
	philosopher(philo_data, &data);
	// free(philo_data);
	return (0);
}
/*
	Threads : nouveaux processus executant une fonction.
pthread_join est un point ou les threads s'attendent.
	Mutex : mutex_lock et mutex_unlock placent une zone qui ne peut etre lu que par un thread a la fois.
*/