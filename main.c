/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 10:02:48 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/08/10 10:22:22 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// manger -> dormir -> penser
// 1 fourchette par philo

typedef struct s_data
{
	long int	number_of_philosopher;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
}	t_data;


// number_of_philosophers	time_to_die 	time_to_eat		time_to_sleep
int	main(int argc, char *argv[])
{
	
}