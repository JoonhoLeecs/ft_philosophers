/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:08:22 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/19 13:57:31 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitoring_routine(void *arg)
{
	t_philo			*philos;
	t_monitor_env	env;

	philos = (t_philo *)arg;
	pthread_mutex_lock(&philos->share->all_alive_lock);
	env.t_last_eat = philos->share->t_start;
	pthread_mutex_unlock(&philos->share->all_alive_lock);
	usleep(philo_max(T_OFFSET, philos->share->n_philo * 5));
	env.check = 0;
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
			pthread_mutex_lock(&(philos[env.i].pub_t_last_eat_lock));
			gettimeofday(&(env.time), NULL);
			env.t_last_eat = philos[env.i].pub_t_last_eat;
			pthread_mutex_unlock(&(philos[env.i].pub_t_last_eat_lock));
			if (env.t_last_eat.tv_sec != 0
				&& get_utime_diff(env.time, env.t_last_eat) > philos->share->t_die)
			{
				pthread_mutex_lock(&philos->share->all_alive_lock);
				if (philos->share->all_alive != ANY_DEAD)
					printf("%ld %d died\n", get_utime_diff(env.time, env.t_last_eat), env.i);
					// philo_print(get_utime_diff(env.time, env.t_last_eat), env.i, DIE);
				philos->share->all_alive = ANY_TO_DIE;
				pthread_mutex_unlock(&philos->share->all_alive_lock);
				env.check = philos->share->n_philo + 1;
				return (NULL);
			}
			else if (env.pub_alive == DEAD)
				env.check = philos->share->n_philo + 2;
			else if (env.pub_alive == DONE_EAT)
				env.check++;
			env.i++;
		}
		usleep(5 * T_OFFSET);
	}
	check_all_done(env, philos);
	return (NULL);
}

void	check_pub_values(t_philo *philos, t_monitor_env *env, int *unit_time)
{
	pthread_mutex_lock(&(philos[env->i].pub_alive_lock));
	env->pub_alive = philos[env->i].pub_alive;
	pthread_mutex_unlock(&(philos[env->i].pub_alive_lock));
	pthread_mutex_lock(&philos->share->all_alive_lock);
	pthread_mutex_lock(&(philos[env->i].pub_t_last_eat_lock));
	env->t_last_eat = philos[env->i].pub_t_last_eat;
	pthread_mutex_unlock(&(philos[env->i].pub_t_last_eat_lock));
	gettimeofday(&(env->time), NULL);
	if (env->t_last_eat.tv_sec != 0
		&& get_utime_diff(env->time, env->t_last_eat) > philos->share->t_die)
	{
		*unit_time = T_UNIT / 2;
		if (philos->share->all_alive != ANY_DEAD)
			philo_print(get_mtime_diff(env->time, env->t_last_eat), env->i, DIE);
		philos->share->all_alive = ANY_TO_DIE;
		env->check = philos->share->n_philo + 1;
	}
	else if (env->pub_alive == DEAD)
		env->check = philos->share->n_philo + 2;
	else if (env->pub_alive == DONE_EAT)
		env->check++;
	pthread_mutex_unlock(&philos->share->all_alive_lock);
}

void	check_all_done(t_monitor_env env, t_philo *philos)
{
	// if (env.check == philos->share->n_philo + 1)
	// {
	// 	pthread_mutex_lock(&philos->share->all_alive_lock);
	// 	if (philos->share->all_alive != ANY_DEAD)
	// 		philo_print(get_mtime_diff(env.time, env.t_last_eat), env.i, DIE);
	// 	philos->share->all_alive = ANY_TO_DIE;
	// 	pthread_mutex_unlock(&philos->share->all_alive_lock);
	// }
	// else if (env.check == philos->share->n_philo)
	if (env.check == philos->share->n_philo)
	{
		pthread_mutex_lock(&philos->share->all_alive_lock);
		philos->share->all_alive = ALL_DONE_EAT;
		pthread_mutex_unlock(&philos->share->all_alive_lock);
	}
}
