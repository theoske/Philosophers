/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/08/12 17:54:36 by tkempf-e         ###   ########.fr       */
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
	struct timeval	start;
	struct timeval	end;
}	t_data;

typedef struct s_philo_data
{
	int			name;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	int			nbr_of_forks_held;
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

void	philo_init(t_data *data, t_philo_data *philo_data[])
{
	int		i;

	i = 0;
	while (i < data->number_of_philo)//segfault over 1
	{
		philo_data[i]->name = i;
		philo_data[i]->nbr_of_forks_held = 0;
		philo_data[i]->time_to_die = data->initial_time_to_die;
		philo_data[i]->time_to_eat = data->initial_time_to_eat;
		philo_data[i]->time_to_sleep = data->initial_time_to_sleep;
		i++;
	}
}

long int	time_diff(struct timeval *start, struct timeval *end)
{
	return (end->tv_usec - start->tv_usec);
}

void	take_fork(t_philo_data *philo_data, t_data *data)
{
	printf("%ld   %d has taken a fork\n", time_diff(&data->start, &data->end), philo_data->name);
}

void	eat(t_philo_data *philo_data, t_data *data)
{
	printf("%ld   %d is eating\n", time_diff(&data->start, &data->end), philo_data->name);
}

void	sleeping(t_philo_data *philo_data, t_data *data)
{
	printf("%ld   %d is sleeping\n", time_diff(&data->start, &data->end), philo_data->name);
}

void	think(t_philo_data *philo_data, t_data *data)
{
	printf("%ld   %d is thinking\n", time_diff(&data->start, &data->end), philo_data->name);
}

void	died(t_philo_data *philo_data, t_data *data)
{
	printf("%ld   %d died\n", time_diff(&data->start, &data->end), philo_data->name);
}

void	philosopher(t_philo_data **philo_data, t_data *data)
{
	int		i;

	i = 0;
	while (data->times_each_philo_must_eat != 0)
	{
		if (i % 2 == 1)
		{
			i = 0;
			data->times_each_philo_must_eat--;
		}
		else
			i = 1;
		while (i < data->number_of_philo)
		{
			take_fork(philo_data[i], data);
			take_fork(philo_data[i], data);
			eat(philo_data[i], data);
			sleeping(philo_data[i], data);
			think(philo_data[i], data);
			i += 2;
		}
	}
}

// take in account time to die/eat/sleep
// number_of_philos		time_to_die 	time_to_eat		time_to_sleep	[times_each_philo_must_eat]
int	main(int argc, char *argv[])
{
	t_data				data;
	t_philo_data		*philo_data;
	pthread_t			*philo;
	pthread_mutex_t		mutex;

	if (arguments_checker(argc, argv, &data) == -1)
		return (ft_error());
	philo_data = 0;
	philo = malloc(sizeof(pthread_t) * data.number_of_philo); // creer autant de threads que de philo
	philo_data = malloc(sizeof(t_philo_data) * data.number_of_philo);// marche pas
	philo_init(&data, &philo_data);
	// philosopher(&philo_data, &data);
	free (philo);
	free (philo_data);
	return (0);
}

/*
	Threads : nouveaux processus executant une fonction.
pthread_join est un point ou les threads s'attendent.
	Mutex : mutex_lock et mutex_unlock placent une zone qui ne peut etre lu que par un thread a la fois.
*/