/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cant_eat_in_time.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 19:17:27 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/10/20 19:22:24 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cant_eat_in_time(t_philo_data *philo)
{
	long long int	tmp;

	pthread_mutex_lock(&philo->data->talk);
	tmp = gettime() - philo->time_now - philo->time_to_die;
	if (tmp < 0)
		tmp *= -1000;
	else
	tmp *= 1000;
	usleep(tmp);
	philo->data->is_dead--;
	if (philo->data->is_dead == -1)
		printf("%lld	%d died.\n",
			gettime() - philo->time_now, philo->name);
	pthread_mutex_unlock(&philo->data->talk);
}
