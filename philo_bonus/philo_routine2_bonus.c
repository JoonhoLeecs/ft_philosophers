/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine2_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 14:36:59 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_sleep(t_philo *philo, t_timeval time)
{
	if (philo->status == TO_SLEEP)
	{
		philo->t_last_sleep = time;
		philo->status = SLEEPING;
		return (SLEEP);
	}
	else if (get_utime_diff(time, philo->t_last_sleep)
		> philo->share->t_sleep)
	{
		usleep(2 * T_OFFSET);
		philo->status = TO_THINK;
		return (SKIP);
	}
	return (NONE);
}

int	philo_think(t_philo *philo, t_timeval time)
{
	if (philo->status == TO_THINK)
	{
		philo->status = THINKING;
		return (THINK);
	}
	else
		return (take_forks(philo, time));
}

int	take_forks(t_philo *philo, t_timeval time)
{
	if (philo->n_forks == 0 || philo->n_forks == 1)
		sem_wait(philo->share->forks_sem);
	else
		return (NONE);
	gettimeofday(&time, NULL);
	philo->n_forks += 1;
	if (check_starvation(philo, time) == DEAD)
		return (NONE);
	philo_printf(get_mtime_diff(time, philo->share->t_start),
		FORK, philo);
	if (philo->ind == 0 && philo->done_check_forks < 6)
	{
		pthread_mutex_lock(&philo->two_forks_lock);
		philo->have_two_forks += philo->n_forks;
		pthread_mutex_unlock(&philo->two_forks_lock);
		philo->done_check_forks += philo->n_forks;
	}
	if (philo->n_forks == 2)
		philo->status = TO_EAT;
	return (SKIP);
}

void	put_back_forks(t_philo *philo)
{
	if (philo->ind == 0 && philo->done_check_forks > 5)
	{
		pthread_mutex_destroy(&philo->two_forks_lock);
		pthread_join(philo->forks_check, NULL);
	}
	if (philo->n_forks == 2)
	{
		sem_post(philo->share->forks_sem);
		sem_post(philo->share->forks_sem);
		philo->n_forks -= 2;
		return ;
	}
	else if (philo->n_forks == 1)
	{
		sem_post(philo->share->forks_sem);
		philo->n_forks -= 1;
	}
}

int	refresh_unit_time(t_philo *philo, t_timeval time)
{
	long		t_left_min;
	long		t_left_state;
	long		t_left_die;

	if (philo->status == TO_EAT || philo->status == TO_SLEEP)
		return (T_UNIT / 2);
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
