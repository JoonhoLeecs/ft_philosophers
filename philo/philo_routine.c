/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/16 18:15:15 by joonhlee         ###   ########.fr       */
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
	routine_init(philo, philo_actions);
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
			if (philo->msg != SKIP)
				philo_printf(get_mtime_diff(time, philo->share->t_start),
					philo->msg, philo);
			continue ;
		}
		usleep(unit_time);
	}
	return (NULL);
}

void	routine_init(t_philo *philo, \
		int (*actions[])(t_philo *, t_timeval time))
{
	actions[TO_EAT] = &philo_eat;
	actions[TO_SLEEP] = &philo_sleep;
	actions[TO_THINK] = &philo_think;
	actions[EATING] = &philo_eat;
	actions[SLEEPING] = &philo_sleep;
	actions[THINKING] = &philo_think;
	gettimeofday(&(philo->t_last_eat), NULL);
	philo->t_last_sleep = philo->t_last_eat;
	pthread_mutex_lock(&philo->pub_t_last_eat_lock);
	philo->pub_t_last_eat = philo->t_last_eat;
	pthread_mutex_unlock(&philo->pub_t_last_eat_lock);
	if (philo->ind % 2 == 0)
		usleep(philo_max(T_OFFSET, philo->share->n_philo * 5));
}

int	check_starvation(t_philo *philo, t_timeval time)
{
	if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_die)
	{
		philo_printf(get_mtime_diff(time, philo->share->t_start),
			DIE, philo);
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
		pthread_mutex_lock(&philo->pub_t_last_eat_lock);
		philo->pub_t_last_eat = time;
		pthread_mutex_unlock(&philo->pub_t_last_eat_lock);
		philo->status = EATING;
		return (EAT);
	}
	else if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_eat)
	{
		if (philo->n_forks > 0)
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
