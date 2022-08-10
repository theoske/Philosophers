/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/08/10 13:27:13 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


// eat -> sleep -> think		#SigmaPhilosopherGrindset
// 1 fork per philosopher

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
	long int	number_of_philosopher;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	long int	times_each_philosopher_must_eat;
}	t_data;

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
	data->number_of_philosopher = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else if (argc == 5)
		data->times_each_philosopher_must_eat = 0;
	return (0);
}

int	ft_error(void)
{
	printf("Arguments Error\n");
	return (-1);
}

//voir mutex et threads
// number_of_philosophers	time_to_die 	time_to_eat		time_to_sleep	[times_each_philosopher_must_eat]
int	main(int argc, char *argv[])
{
	t_data	data;

	if (arguments_checker(argc, argv, &data) == -1)
		return (ft_error());
	else
		printf("bon\n");
	printf("data values : %d %d %d %d %d\n", data.number_of_philosopher, data.time_to_die, data.time_to_eat, data.time_to_sleep, data.times_each_philosopher_must_eat);
	
	return (0);
}