/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 14:34:43 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_think(t_philo *philo, t_timeval time)
{
	if (philo->status == TO_THINK)
	{
		usleep(2 * T_OFFSET);
		philo->status = THINKING;
		return (THINK);
	}
	else
		return (take_forks(philo, time));
}

int	take_forks(t_philo *philo, t_timeval time)
{
	int	fork_ind;

	if (philo->n_forks == 1 && philo->first_fork == philo->second_fork)
		return (NONE);
	if (philo->n_forks == 0)
		fork_ind = philo->first_fork;
	else if (philo->n_forks == 1)
		fork_ind = philo->second_fork;
	else
		return (NONE);
	pthread_mutex_lock(philo->share->fork_locks + fork_ind);
	gettimeofday(&time, NULL);
	philo->n_forks += 1;
	if (check_starvation(philo, time) == DEAD)
		return (NONE);
	philo_printf(get_mtime_diff(time, philo->share->t_start),
		FORK, philo);
	*(philo->share->forks + fork_ind) = philo->ind;
	if (philo->n_forks == 2)
		philo->status = TO_EAT;
	return (SKIP);
}

void	put_back_forks(t_philo *philo)
{
	if (philo->n_forks == 2)
	{
		*(philo->share->forks + philo->second_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->second_fork);
		*(philo->share->forks + philo->first_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		philo->n_forks -= 2;
	}
	else if (philo->n_forks == 1)
	{
		*(philo->share->forks + philo->first_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		philo->n_forks -= 1;
	}
}

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

void	philo_printf(long time, t_msg msg, t_philo *philo)
{
	pthread_mutex_lock(&philo->share->all_alive_lock);
	if (philo->share->all_alive != ANY_DEAD)
	{
		if (msg == DIE)
		{
			printf("%ld %d died\n", time, philo->ind + 1);
			philo->share->all_alive = ANY_DEAD;
		}
		else if (msg == THINK)
			printf("%ld %d is thinking\n", time, philo->ind + 1);
		else if (msg == FORK)
			printf("%ld %d has taken a fork\n", time, philo->ind + 1);
		else if (msg == EAT)
			printf("%ld %d is eating\n", time, philo->ind + 1);
		else if (msg == SLEEP)
			printf("%ld %d is sleeping\n", time, philo->ind + 1);
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
