/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 14:56:25 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/10/15 15:15:07 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

typedef struct s_data
{
	long long int	number_of_philo;
	long long int	initial_time_to_die;
	long long int	initial_time_to_eat;
	long long int	initial_time_to_sleep;
	int				is_dead;
	pthread_mutex_t	talk;
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

int				ft_finder(const char *nptr);
int				ft_atoi(const char *nptr);
int				number_checker(char *argv[]);
int				arguments_checker(int argc, char *argv[], t_data *data);
int				ft_error(void);
long long int	gettime(void);
void			philo_init(t_data *data, t_philo_data *philo,
					int argc, char **argv);
void			talking(t_philo_data *philo, int option);
int				died(t_philo_data *philo);
void			take_fork(t_philo_data *philo);
void			eating(t_philo_data *philo);
void			sleeping(t_philo_data *philo);
void			thinking(t_philo_data *philo);
void			philosopher(t_philo_data *philo);

#endif
