/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/08/10 11:15:43 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


// eat -> sleep -> think		#SigmaPhilosopherGrindset
// 1 fork per philosopher

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
	Converting them from char* to int.
*/
int	argchecker(int argc, char *argv[], t_data *data)
{
	if (number_checker(argv) == -1)
		return (-1);
}

int	ft_error(void)
{
	printf("Arguments Error\n");
	return (-1);
}

// number_of_philosophers	time_to_die 	time_to_eat		time_to_sleep	[times_each_philosopher_must_eat]
int	main(int argc, char *argv[])
{
	t_data	data;

	if (arg_checker(argc, argv, &data) == -1)
		return (ft_error);
}