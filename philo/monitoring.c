/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:08:22 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/26 09:33:59 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitoring_full_routine(void *arg)
{
	t_philo			*philos;
	t_monitor_env	env;

	philos = (t_philo *)arg;
	init_monitoring(&env, philos);
	while (env.check < philos->share->n_philo)
	{
		env.check = 0;
		env.i = 0;
		while (env.i < philos->share->n_philo
			&& env.check < philos->share->n_philo)
		{
			pthread_mutex_lock(&(philos[env.i].pub_alive_lock));
			env.pub_alive = philos[env.i].pub_alive;
			pthread_mutex_unlock(&(philos[env.i].pub_alive_lock));
			if (env.pub_alive == DEAD)
				env.check = philos->share->n_philo + 1;
			else if (env.pub_alive == DONE_EAT)
				env.check++;
			env.i++;
		}
		usleep(5 * T_OFFSET);
	}
	check_all_done(env, philos);
	return (NULL);
}

void	*monitoring_starve_routine(void *arg)
{
	t_philo			*philos;
	t_monitor_env	env;

	philos = (t_philo *)arg;
	init_monitoring(&env, philos);
	while (env.check == 0)
	{
		env.i = 0;
		while (env.i < philos->share->n_philo && env.check == 0)
		{
			pthread_mutex_lock(&(philos[env.i].pub_t_last_eat_lock));
			env.t_last_eat = philos[env.i].pub_t_last_eat;
			pthread_mutex_unlock(&(philos[env.i].pub_t_last_eat_lock));
			gettimeofday(&(env.time), NULL);
			if (env.t_last_eat.tv_sec != 0 && get_utime_diff(env.time,
					env.t_last_eat) > philos->share->t_die)
				check_any_to_die(&env, philos);
			env.i++;
		}
		usleep(5 * T_OFFSET);
	}
	return (NULL);
}

void	init_monitoring(t_monitor_env *env, t_philo *philos)
{
	pthread_mutex_lock(&philos->share->all_alive_lock);
	env->t_last_eat = philos->share->t_start;
	pthread_mutex_unlock(&philos->share->all_alive_lock);
	usleep(6 * T_OFFSET + 14 * philos->share->n_philo);
	env->check = 0;
}

void	check_all_done(t_monitor_env env, t_philo *philos)
{
	if (env.check == philos->share->n_philo)
	{
		pthread_mutex_lock(&philos->share->all_alive_lock);
		if (philos->share->all_alive == ALL_ALIVE)
			philos->share->all_alive = ALL_DONE_EAT;
		pthread_mutex_unlock(&philos->share->all_alive_lock);
	}
}

void	check_any_to_die(t_monitor_env *env, t_philo *philos)
{
	pthread_mutex_lock(&philos->share->all_alive_lock);
	if (philos->share->all_alive == ALL_ALIVE)
	{
		philos->share->all_alive = ANY_DEAD;
		philo_print(get_utime_diff(env->time, philos->share->t_start) / 1000,
			env->i + 1, DIE);
	}
	pthread_mutex_unlock(&philos->share->all_alive_lock);
	env->check = 1;
}
