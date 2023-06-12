/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:08:22 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/12 14:23:10 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitoring_routine(void *arg)
{
	t_philo	*philos;
	t_alive	pub_alive;
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
			pub_alive = philos[i].pub_alive;
			pthread_mutex_unlock(&(philos[i].pub_alive_lock));
			if (pub_alive == DONE_EAT)
				check++;
			else if (pub_alive == DEAD)
				check = philos->share->n_philo + 1;
			i++;
		}
		usleep(1 * T_UNIT);
	}
	if (check == philos->share->n_philo)
	{
		pthread_mutex_lock(&philos->share->all_alive_lock);
		philos->share->all_alive = ALL_DONE_EAT;
		pthread_mutex_unlock(&philos->share->all_alive_lock);
	}
	return (NULL);
}
