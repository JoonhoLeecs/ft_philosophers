/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/19 21:28:31 by joonhlee         ###   ########.fr       */
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
	if (philo->first_fork == philo->second_fork && philo->n_forks < 1)
	{
		pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
		if (*(philo->share->forks + philo->first_fork) == 0)
		{
			philo->n_forks += 1;
			*(philo->share->forks + philo->first_fork) = philo->ind + 1;
		}
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		gettimeofday(&time, NULL);
		if (check_starvation(philo, time) == DEAD)
			return (NONE);
		philo_printf(get_mtime_diff(time, philo->share->t_start),
			FORK, philo);
		return (SKIP);
	}
	else if (philo->first_fork == philo->second_fork && philo->n_forks == 1)
		return (NONE);
	while (philo->n_forks < 2)
	{
		pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
		if (*(philo->share->forks + philo->first_fork) == 0)
		{
			philo->n_forks += 1;
			*(philo->share->forks + philo->first_fork) = philo->ind + 1;
		}
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		// usleep(T_UNIT * 5);
		if (philo->n_forks == 1)
		{
			pthread_mutex_lock(philo->share->fork_locks + philo->second_fork);
			if (*(philo->share->forks + philo->second_fork) == 0)
			{
				philo->n_forks += 1;
				*(philo->share->forks + philo->second_fork) = philo->ind + 1;
			}
			else
				put_back_forks(philo);
			pthread_mutex_unlock(philo->share->fork_locks + philo->second_fork);
		}
		// pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		gettimeofday(&time, NULL);
		if (check_starvation(philo, time) == DEAD)
			return (NONE);
		if (philo->n_forks == 2)
			break ;
		// usleep(T_UNIT * 5);
		usleep(refresh_unit_time2(philo, time));
	}
	philo_printf(get_mtime_diff(time, philo->share->t_start),
		FORK, philo);
	philo_printf(get_mtime_diff(time, philo->share->t_start),
		FORK, philo);
	philo->status = TO_EAT;
	return (SKIP);
}

void	put_back_forks(t_philo *philo)
{
	if (philo->n_forks == 0)
		return ;
	if (philo->n_forks == 2)
	{
		pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
		*(philo->share->forks + philo->first_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		pthread_mutex_lock(philo->share->fork_locks + philo->second_fork);
		*(philo->share->forks + philo->second_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->second_fork);
		philo->n_forks -= 2;
	}
	else if (philo->n_forks == 1)
	{
		pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
		*(philo->share->forks + philo->first_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		philo->n_forks -= 1;
	}
}

// int	take_forks(t_philo *philo, t_timeval time)
// {
// 	pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
// 	if (*(philo->share->forks + philo->first_fork) == 0)
// 	{
// 		philo->n_forks += 1;
// 		*(philo->share->forks + philo->first_fork) = philo->ind + 1;
// 	}
// 	pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);

// 	pthread_mutex_lock(philo->share->fork_locks + philo->second_fork);
// 	if (*(philo->share->forks + philo->second_fork) == 0)
// 	{
// 		philo->n_forks += 1;
// 		*(philo->share->forks + philo->second_fork) = philo->ind + 1;
// 	}
// 	pthread_mutex_unlock(philo->share->fork_locks + philo->second_fork);
// 	if (philo->n_forks == 2
// 		|| (philo->n_forks == 1 && philo->first_fork == philo->second_fork))
// 	{
// 		gettimeofday(&time, NULL);
// 		if (check_starvation(philo, time) == DEAD)
// 			return (NONE);
// 		philo_printf(get_mtime_diff(time, philo->share->t_start),
// 			FORK, philo);
// 		if (philo->n_forks == 1)
// 		{
// 			usleep(philo->share->t_die);
// 			return (SKIP);
// 		}
// 		philo_printf(get_mtime_diff(time, philo->share->t_start),
// 			FORK, philo);
// 		philo->status = TO_EAT;
// 		return (SKIP);
// 	}
// 	else
// 	{
// 		put_back_forks(philo);
// 		return (NONE);
// 	}
// }

