/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:08:22 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/12 08:41:46 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitoring_routine(void *arg)
{
	t_philo	*philos;
	int		i;
	int		check;

	philos = (t_philo *)arg;
	usleep(philo_max(T_OFFSET, philos->share->n_philo * 3));
	check = 0;
	while (check < philos->share->n_philo)
	{
		check = 0;
		i = 0;
		while (i < philos->share->n_philo)
		{
			pthread_mutex_lock(&(philos[i].pub_alive_lock));
			if (philos[i].pub_alive == DONE_EAT)
				check++;
			pthread_mutex_unlock(&(philos[i].pub_alive_lock));
			i++;
		}
		usleep(1 * T_UNIT);
	}
	pthread_mutex_lock(&philos->share->all_alive_lock);
	philos->share->all_alive = ALL_DONE_EAT;
	pthread_mutex_unlock(&philos->share->all_alive_lock);
	return (NULL);
}
