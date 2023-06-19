/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/19 11:07:52 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_sleep(t_philo *philo, t_timeval time)
{
	if (philo->status == TO_SLEEP)
	{
		philo->t_last_sleep = time;
		philo->status = SLEEPING;
		return (SLEEP);
	}
	else if (get_utime_diff(time, philo->t_last_sleep) > philo->share->t_sleep)
	{
		philo->status = TO_THINK;
		return (SKIP);
	}
	return (NONE);
}

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
	*(philo->share->forks + fork_ind) = philo->ind;
	philo_printf(get_mtime_diff(time, philo->share->t_start),
		FORK, philo);
	if (philo->n_forks == 2)
		philo->status = TO_EAT;
	return (SKIP);
}

void	put_back_forks(t_philo *philo)
{
	if (philo->n_forks == 0)
		return ;
	if (philo->n_forks == 2)
	{
		*(philo->share->forks + philo->first_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		*(philo->share->forks + philo->second_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->second_fork);
		philo->n_forks -= 2;
	}
	else if (philo->n_forks == 1)
	{
		*(philo->share->forks + philo->first_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		philo->n_forks -= 1;
	}
}
