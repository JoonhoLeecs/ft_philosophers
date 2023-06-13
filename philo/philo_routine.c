/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/13 08:50:48 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	int			(*philo_actions[6])(t_philo *, t_timeval time);
	int			unit_time;
	t_timeval	time;

	philo = (t_philo *)arg;
	philo_actions[TO_EAT] = &philo_eat;
	philo_actions[TO_SLEEP] = &philo_sleep;
	philo_actions[TO_THINK] = &philo_think;
	philo_actions[EATING] = &philo_eat;
	philo_actions[SLEEPING] = &philo_sleep;
	philo_actions[THINKING] = &philo_think;
	gettimeofday(&(philo->t_last_eat), NULL);
	philo->t_last_sleep = philo->t_last_eat;
	if (philo->ind % 2 == 0)
		usleep(philo_max(100, philo->share->n_philo * 5));
	while (philo->alive != DEAD)
	{
		gettimeofday(&time, NULL);
		unit_time = T_UNIT;
		if (check_starvation(philo, time) == DEAD)
			break ;
		unit_time = refresh_unit_time(philo, time);
		philo->msg = philo_actions[(int)philo->status](philo, time);
		if (philo->msg != NONE)
		{
			unit_time = T_UNIT;
			if (philo->msg != SKIP)
				philo_printf(get_mtime_diff(time, philo->share->t_start),
					philo->msg, philo);
			continue ;
		}
		usleep(unit_time);
	}
	return (NULL);
}
// printf("last_eat:  %ld|%d sec\n", philo->t_last_eat.tv_sec,
// 	philo->t_last_eat.tv_usec);
// printf("last_sleep:%ld|%d sec\n", philo->t_last_sleep.tv_sec,
// 	philo->t_last_sleep.tv_usec);

int	check_starvation(t_philo *philo, t_timeval time)
{
	if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_die)
	{
		philo_printf(get_mtime_diff(time, philo->share->t_start),
			DIE, philo);
		put_back_forks(philo);
		philo->alive = DEAD;
		pthread_mutex_lock(&philo->pub_alive_lock);
		philo->pub_alive = DEAD;
		pthread_mutex_unlock(&philo->pub_alive_lock);
	}
	return (philo->alive);
}

int	philo_eat(t_philo *philo, t_timeval time)
{
	if (philo->status == TO_EAT)
	{
		philo->t_last_eat = time;
		philo->status = EATING;
		return (EAT);
	}
	else if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_eat)
	{
		put_back_forks(philo);
		if (philo->share->n_eat > -1)
			philo->n_eat += 1;
		if (philo->share->n_eat > -1 && philo->n_eat == philo->share->n_eat)
		{
			philo->alive = DONE_EAT;
			pthread_mutex_lock(&philo->pub_alive_lock);
			philo->pub_alive = DONE_EAT;
			pthread_mutex_unlock(&philo->pub_alive_lock);
		}
		philo->status = TO_SLEEP;
		return (SKIP);
	}
	return (NONE);
}

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
	if (philo->n_forks == 1 && philo->first_fork == philo->second_fork)
		return (NONE);
	if (philo->n_forks == 0)
		pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
	else
		pthread_mutex_lock(philo->share->fork_locks + philo->second_fork);
	gettimeofday(&time, NULL);
	philo->n_forks += 1;
	if (check_starvation(philo, time) == DEAD)
		return (NONE);
	philo_printf(get_mtime_diff(time, philo->share->t_start),
		FORK, philo);
	if (philo->n_forks == 0)
		*(philo->share->forks + philo->first_fork) = philo->ind;
	else
	{
		*(philo->share->forks + philo->second_fork) = philo->ind;
		philo->status = TO_EAT;
	}
	return (SKIP);
}

void	put_back_forks(t_philo *philo)
{
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

int	refresh_unit_time(t_philo *philo, t_timeval time)
{
	long		t_left_min;
	long		t_left_state;
	long		t_left_die;

	if (philo->status != EATING && philo->status != SLEEPING)
		return (T_UNIT);
	// if (philo->status == TO_EAT || philo->status == TO_SLEEP)
	// 	return (T_UNIT / 2);
	if (philo->status == EATING)
		t_left_state = philo->share->t_eat
			- get_utime_diff(time, philo->t_last_eat);
	else
		t_left_state = philo->share->t_sleep
			- get_utime_diff(time, philo->t_last_sleep);
	t_left_die = philo->share->t_die - get_utime_diff(time, philo->t_last_eat);
	t_left_min = philo_min(t_left_state, t_left_die) / 2;
	if (T_UNIT < t_left_min)
		return ((int) t_left_min);
	else
		return (T_UNIT);
}

void	philo_printf(long time, t_msg msg, t_philo *philo)
{
	t_all_alive	all_alive;

	pthread_mutex_lock(&philo->share->all_alive_lock);
	all_alive = philo->share->all_alive;
	if (all_alive != ANY_DEAD)
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
	if (all_alive != ALL_ALIVE)
	{
		put_back_forks(philo);
		philo->alive = DEAD;
	}
	pthread_mutex_unlock(&philo->share->all_alive_lock);
	philo->msg = NONE;
}
