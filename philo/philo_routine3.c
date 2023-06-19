/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/19 11:11:27 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	refresh_unit_time(t_philo *philo, t_timeval time)
{
	long		t_left_min;
	long		t_left_state;
	long		t_left_die;

	if (philo->status != EATING && philo->status != SLEEPING)
		return (T_UNIT);
	if (philo->status == EATING)
		t_left_state = philo->share->t_eat
			- get_utime_diff(time, philo->t_last_eat);
	else
		t_left_state = philo->share->t_sleep
			- get_utime_diff(time, philo->t_last_sleep);
	t_left_die = philo->share->t_die - get_utime_diff(time, philo->t_last_eat);
	t_left_min = (philo_min(t_left_state, t_left_die) * 3) / 4;
	if (philo->status == EATING)
		t_left_min = philo_min(t_left_min, 7000);
	if (T_UNIT < t_left_min)
		return ((int) t_left_min);
	else
		return (T_UNIT);
}

void	philo_printf(long time, t_msg msg, t_philo *philo)
{
	pthread_mutex_lock(&philo->share->all_alive_lock);
	if (philo->share->all_alive == ANY_TO_DIE)
	{
		if (philo->n_forks > 0)
			put_back_forks(philo);
		philo->alive = DEAD;
		pthread_mutex_unlock(&philo->share->all_alive_lock);
		philo->msg = NONE;
		return ;
	}
	// if (philo->share->all_alive != ANY_DEAD && philo->share->all_alive != ANY_TO_DIE)
	if (philo->share->all_alive != ANY_DEAD)
	{
		philo_print(time, philo->ind + 1, msg);
		if (msg == DIE)
			philo->share->all_alive = ANY_DEAD;
	}
	if (philo->share->all_alive != ALL_ALIVE)
	{
		if (philo->n_forks > 0)
			put_back_forks(philo);
		philo->alive = DEAD;
	}
	pthread_mutex_unlock(&philo->share->all_alive_lock);
	philo->msg = NONE;
}
