/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/19 19:58:24 by joonhlee         ###   ########.fr       */
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
		if (philo->status == THINKING && philo->msg == NONE)
			// unit_time = T_OFFSET * 4;
			unit_time = refresh_unit_time2(philo, time);
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
	actions[TO_EAT] = &philo_to_eat;
	actions[TO_SLEEP] = &philo_sleep;
	actions[TO_THINK] = &philo_think;
	actions[EATING] = &philo_eating;
	actions[SLEEPING] = &philo_sleep;
	actions[THINKING] = &philo_think;
	pthread_mutex_lock(&philo->share->all_alive_lock);
	philo->t_last_eat = philo->share->t_start;
	pthread_mutex_unlock(&philo->share->all_alive_lock);
	philo->t_last_sleep = philo->t_last_eat;
		// usleep(philo_max(T_OFFSET, philo->share->n_philo * 5));
	if (philo->ind % 2 == 0)
		usleep(philo_max(T_OFFSET, philo->share->n_philo * 4) + 4 * philo->ind);
	else
		usleep(4 * philo->ind);
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

int	philo_to_eat(t_philo *philo, t_timeval time)
{
	philo->t_last_eat = time;
	philo->status = EATING;
	return (EAT);
}

int	philo_eating(t_philo *philo, t_timeval time)
{
	if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_eat)
	{
		if (philo->n_forks > 0)
			put_back_forks(philo);
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
