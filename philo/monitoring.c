/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:08:22 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/13 12:32:59 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitoring_routine(void *arg)
{
	t_philo			*philos;
	t_monitor_env	env;

	philos = (t_philo *)arg;
	usleep(philo_max(T_OFFSET, philos->share->n_philo * 5));
	env.check = 0;
	while (env.check < philos->share->n_philo)
	{
		env.check = 0;
		env.i = 0;
		while (env.i < philos->share->n_philo)
		{
			pthread_mutex_lock(&(philos[env.i].pub_alive_lock));
			env.pub_alive = philos[env.i].pub_alive;
			pthread_mutex_unlock(&(philos[env.i].pub_alive_lock));
			if (env.pub_alive == DONE_EAT)
				env.check++;
			else if (env.pub_alive == DEAD)
				env.check = philos->share->n_philo + 1;
			env.i++;
		}
		usleep(2 * T_UNIT);
	}
	check_all_done(env.check, philos);
	return (NULL);
}

void	check_all_done(int check, t_philo *philos)
{
	if (check == philos->share->n_philo)
	{
		pthread_mutex_lock(&philos->share->all_alive_lock);
		philos->share->all_alive = ALL_DONE_EAT;
		pthread_mutex_unlock(&philos->share->all_alive_lock);
	}
}
