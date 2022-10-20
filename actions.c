/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 15:06:01 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/10/20 19:32:58 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
options :
	0 : take fork
	1 : eat
	2 : sleep
	3 : think
*/
void	talking(t_philo_data *philo, int option)
{
	pthread_mutex_lock(&philo->data->talk);
	if (philo->data->is_dead == 0)
	{
		if (option == 0)
			printf("%lld	%d has taken a fork.\n",
				gettime() - philo->time_now, philo->name);
		else if (option == 1)
			printf("%lld	%d is eating.\n",
				gettime() - philo->time_now, philo->name);
		else if (option == 2)
			printf("%lld	%d is sleeping.\n",
				gettime() - philo->time_now, philo->name);
		else if (option == 3)
			printf("%lld	%d is thinking.\n",
				gettime() - philo->time_now, philo->name);
	}
	pthread_mutex_unlock(&philo->data->talk);
}

int	died(t_philo_data *philo)
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
			printf("%lld	%d died.\n",
				gettime() - philo->time_now, philo->name);
		pthread_mutex_unlock(&philo->data->talk);
		return (-1);
	}
	return (0);
}

void	take_fork(t_philo_data *philo)
{
	if (philo->name % 2 == 1 && philo->time_eaten == 0)
	{
		if (philo->data->initial_time_to_die < philo->data->initial_time_to_eat)
			usleep(philo->time_to_die * 1000);
		else
			usleep(philo->time_to_eat * 9 / 10);
	}
	if (philo->time_eaten == 1
		&& philo->data->initial_time_to_eat * 2
		> philo->data->initial_time_to_die)
		cant_eat_in_time(philo);
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(philo->right_fork);
	if (died(philo) == 0)
	{
		talking(philo, 0);
		talking(philo, 0);
	}
}

void	eating(t_philo_data *philo)
{
	take_fork(philo);
	if (died(philo) == 0)
		talking(philo, 1);
	philo->last_meal = gettime();
	if (philo->data->initial_time_to_die < philo->data->initial_time_to_eat)
		usleep(philo->data->initial_time_to_die * 1000);
	else
		usleep(philo->time_to_eat);
	philo->time_eaten++;
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleeping(t_philo_data *philo)
{
	talking(philo, 2);
	if (philo->data->initial_time_to_eat + philo->data->initial_time_to_sleep
		> philo->data->initial_time_to_die)
		cant_eat_in_time(philo);
	else
		usleep(philo->time_to_sleep);
}
