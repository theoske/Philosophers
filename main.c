/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/10/20 23:55:38 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	one_philo(t_philo_data *philo)
{
	pthread_mutex_lock(&philo->fork);
	talking(philo, 0);
	usleep(philo->time_to_die * 1000);
	pthread_mutex_lock(&philo->data->talk);
	philo->data->is_dead--;
	if (philo->data->is_dead == -1)
		printf("%lld	%d died.\n",
			gettime() - philo->time_now, philo->name);
	pthread_mutex_unlock(&philo->data->talk);
	pthread_mutex_unlock(&philo->fork);
}

void	philosopher(t_philo_data *philo)
{
	philo->time_now = gettime();
	if (philo->data->number_of_philo == 1)
		one_philo(philo);
	else
	{
		while (philo->time_eaten != philo->times_each_philo_must_eat)
		{
			eating(philo);
			if (philo->time_eaten != philo->times_each_philo_must_eat)
			{
				if (died(philo) == 0)
					sleeping(philo);
				if (died(philo) == 0)
					talking(philo, 3);
				if (died(philo) != 0)
					break ;
			}
		}
	}
}

/*
1 800 200 200 not eat and should die
5 800 200 200 no die
5 800 200 200 7 no die stop after 7 eat
4 410 200 200 no die
4 310 200 100 one should die
2 philo to check times -> no delay above 10
*/
int	main(int argc, char *argv[])
{
	t_data				data;
	t_philo_data		philo[250];
	int					*i;

	if (arguments_checker(argc, argv, &data) == -1)
		return (ft_error());
	i = malloc(6 * sizeof(int));
	memset(i, -1, 6 * sizeof(int));
	while (++i[0] < data.number_of_philo)
		philo_init(&data, &philo[i[0]], argc, argv);
	while (++i[1] < data.number_of_philo)
		pthread_mutex_init(&philo[i[1]].fork, NULL);
	i[2] = 0;
	while (++i[2] < data.number_of_philo)
		philo[i[2]].right_fork = &philo[i[2] - 1].fork;
	philo[0].right_fork = &philo[data.number_of_philo - 1].fork;
	while (++i[3] < data.number_of_philo)
		pthread_create(&philo[i[3]].thread, NULL, (void *)philosopher, &philo[i[3]]);
	while (++i[4] < data.number_of_philo)
		pthread_join(philo[i[4]].thread, NULL);
	while (++i[5] < data.number_of_philo)
		ft_mutex_free(&philo[i[5]]);
	free(i);
	return (0);
}
