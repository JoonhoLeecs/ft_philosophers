/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/26 09:05:24 by joonhlee         ###   ########.fr       */
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
	if (T_UNIT < t_left_min)
		return ((int) t_left_min);
	else
		return (T_UNIT);
}

void	philo_printf(t_msg msg, t_philo *philo)
{
	t_timeval	print_time;

	pthread_mutex_lock(&philo->share->all_alive_lock);
	if (philo->share->all_alive == ALL_ALIVE)
	{
		gettimeofday(&print_time, NULL);
		philo_print(get_utime_diff(print_time, philo->share->t_start) / 1000,
			philo->ind + 1, msg);
		update_time(msg, print_time, philo);
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

void	update_time(t_msg msg, t_timeval print_time, t_philo *philo)
{
	if (msg == DIE)
		philo->share->all_alive = ANY_DEAD;
	else if (msg == EAT)
	{
		pthread_mutex_lock(&philo->pub_t_last_eat_lock);
		philo->pub_t_last_eat = print_time;
		pthread_mutex_unlock(&philo->pub_t_last_eat_lock);
		philo->t_last_eat = print_time;
	}
	else if (msg == SLEEP)
		philo->t_last_sleep = print_time;
}
