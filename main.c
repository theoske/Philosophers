/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/08/17 17:59:46 by theo             ###   ########.fr       */
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
	long int		times_each_philo_must_eat;
	struct timeval	*start;
}	t_data;

typedef struct s_philo_data
{
	int			*name;
	long int	*time_to_die;
	long int	*time_to_eat;
	long int	*time_to_sleep;
	int			*forks_held;
}	t_philo_data;

void	free_philo(t_philo_data *philo_data)
{
	free (philo_data->name);
	free (philo_data->forks_held);
	free (philo_data->time_to_die);
	free (philo_data->time_to_eat);
	free (philo_data->time_to_sleep);
}

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
	if (argc == 6)
		data->times_each_philo_must_eat = ft_atoi(argv[5]);
	else if (argc == 5)
		data->times_each_philo_must_eat = -1;
	gettimeofday(&data->start, NULL);
	return (0);
}

int	ft_error(void)
{
	printf("Arguments Error\n");
	return (-1);
}

void	allocate_philo(t_data *data, t_philo_data *philo_data)
{
	philo_data->name = malloc(sizeof(int) * data->number_of_philo);
	philo_data->forks_held = malloc(sizeof(int) * data->number_of_philo);
	philo_data->time_to_die = malloc(sizeof(long int) * data->number_of_philo);
	philo_data->time_to_eat = malloc(sizeof(long int) * data->number_of_philo);
	philo_data->time_to_sleep = malloc(sizeof(long int) * data->number_of_philo);
}

void	philo_init(t_data *data, t_philo_data *philo_data)
{
	int		i;

	i = 0;
	allocate_philo(data, philo_data);
	while (i < data->number_of_philo)//segfault over 1
	{
		philo_data->name[i] = i + 1;
		philo_data->time_to_die[i] = data->initial_time_to_die;
		philo_data->time_to_eat[i] = data->initial_time_to_eat;
		philo_data->time_to_sleep[i] = data->initial_time_to_sleep;
		i++;
	}
}

long int	time_diff(struct timeval *start)
{
	long int		diff;
	struct timeval	*end;

	gettimeofday(end, NULL);
	diff = (end->tv_sec + (end->tv_usec * 10e6)) - (start->tv_sec + (start->tv_usec * 10e6));
	return (diff);
}

void	take_fork(t_philo_data *philo_data, t_data *data)
{
	philo_data->forks_held++;
	printf("%ld   %d has taken a fork\n", time_diff(&data->start), philo_data->name);
}

//mange un certain temps et doit garder ses fourchettes pendant ce temps
//reset la faim
// void	eat(t_philo_data *philo_data, t_data *data)
// {
// 	printf("%ld   %d is eating\n", time_diff(&data->start, &data->end), philo_data->name);
// }

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
	int		i;
	pthread_t	*thread;

	i = 0;
	while (1)
	{
		// pthread_create(thread, NULL, (void *)take_fork, (void *) &philo_data[i]);
		i++;
	}
}

// take in account time to die/eat/sleep
// number_of_philos		time_to_die 	time_to_eat		time_to_sleep	[times_each_philo_must_eat]
int	main(int argc, char *argv[])
{
	t_data				data;
	t_philo_data		philo_data;

	if (arguments_checker(argc, argv, &data) == -1)
		return (ft_error());
	philo_init(&data, &philo_data);
	gettimeofday(data.start, NULL);
	philosopher(&philo_data, &data);
	free_philo(&philo_data);
	return (0);
}

/*
	Threads : nouveaux processus executant une fonction.
pthread_join est un point ou les threads s'attendent.
	Mutex : mutex_lock et mutex_unlock placent une zone qui ne peut etre lu que par un thread a la fois.
*/