/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/26 09:33:36 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_routine(t_philo *philo)
{
	int			(*philo_actions[6])(t_philo *, t_timeval time);
	int			unit_time;
	t_timeval	time;

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
				philo_printf(philo->msg, philo);
			continue ;
		}
		usleep(unit_time);
	}
	pthread_join(philo->starving_monitor, NULL);
	exit (3 - philo->alive);
}

void	routine_init(t_philo *philo,
		int (*actions[])(t_philo *, t_timeval time))
{
	actions[TO_EAT] = &philo_eat;
	actions[TO_SLEEP] = &philo_sleep;
	actions[TO_THINK] = &philo_think;
	actions[EATING] = &philo_eat;
	actions[SLEEPING] = &philo_sleep;
	actions[THINKING] = &philo_think;
	philo->t_last_eat = philo->share->t_start;
	philo->t_last_sleep = philo->t_last_eat;
	sem_wait(philo->last_eat_sem);
	philo->pub_t_last_eat = philo->t_last_eat;
	sem_post(philo->last_eat_sem);
	if (philo->ind % 2 == 0)
		usleep(3 * T_OFFSET + 7 * philo->share->n_philo);
	if (pthread_create(&philo->starving_monitor, NULL,
			starving_monitor, philo) != 0)
		exit (EXIT_FAILURE);
}

void	*starving_monitor(void *arg)
{
	t_philo		*philo;
	t_timeval	time;
	t_timeval	pub_t_last_eat;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->last_eat_sem);
		pub_t_last_eat = philo->pub_t_last_eat;
		sem_post(philo->last_eat_sem);
		gettimeofday(&time, NULL);
		if (get_utime_diff(time, pub_t_last_eat) > philo->share->t_die)
		{
			sem_post(philo->share->forks_sem);
			return (NULL);
		}
		usleep(5 * T_OFFSET);
	}
}

int	check_starvation(t_philo *philo, t_timeval time)
{
	if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_die)
	{
		philo_printf(DIE, philo);
		if (philo->n_forks > 0)
			put_back_forks(philo);
		philo->alive = DEAD;
		sem_close(philo->share->forks_sem);
		sem_close(philo->last_eat_sem);
		if (philo->share->n_eat > -1)
			sem_close(philo->share->fulls_sem);
	}
	return (philo->alive);
}

int	philo_eat(t_philo *philo, t_timeval time)
{
	if (philo->status == TO_EAT)
	{
		philo->t_last_eat = time;
		sem_wait(philo->last_eat_sem);
		philo->pub_t_last_eat = time;
		sem_post(philo->last_eat_sem);
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
			sem_post(philo->share->fulls_sem);
		}
		philo->status = TO_SLEEP;
		return (SKIP);
	}
	return (NONE);
}
