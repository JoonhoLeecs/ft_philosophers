/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 07:37:15 by joonhlee         ###   ########.fr       */
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
				philo_printf(get_mtime_diff(time, philo->share->t_start),
					philo->msg, philo);
			continue ;
		}
		usleep(unit_time);
	}
	exit (3 - philo->alive);
}
// printf("philo_routines.c:26|time:%ld|ind:%d|status:%d|n_forks%d\n", get_utime_diff(time, philo->share->t_start), philo->ind, philo->status, philo->n_forks);
// printf("last_eat:  %ld|%d sec\n", philo->t_last_eat.tv_sec,
// 	philo->t_last_eat.tv_usec);
// printf("last_sleep:%ld|%d sec\n", philo->t_last_sleep.tv_sec,
// 	philo->t_last_sleep.tv_usec);

void	routine_init(t_philo *philo,
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
	if (philo->ind % 2 == 0)
		usleep(philo_max(T_OFFSET, philo->share->n_philo * 5));
}

int	check_starvation(t_philo *philo, t_timeval time)
{
	if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_die)
	{
		philo_printf(get_mtime_diff(time, philo->share->t_start),
			DIE, philo);
		if (philo->n_forks > 0)
			put_back_forks(philo);
		philo->alive = DEAD;
		sem_close(philo->share->forks_sem);
		sem_close(philo->share->print_sem);
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
	if (philo->n_forks == 0 || philo->n_forks == 1)
		sem_wait(philo->share->forks_sem);
	else
		return (NONE);
	// if (philo->n_forks == 1 && philo->first_fork == philo->second_fork)
	// 	return (NONE);
	gettimeofday(&time, NULL);
	philo->n_forks += 1;
	if (check_starvation(philo, time) == DEAD)
		return (NONE);
	philo_printf(get_mtime_diff(time, philo->share->t_start),
		FORK, philo);
	if (philo->n_forks == 2)
		philo->status = TO_EAT;
	return (SKIP);
}

void	put_back_forks(t_philo *philo)
{
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

void	philo_printf(long time, t_msg msg, t_philo *philo)
{
	sem_wait(philo->share->print_sem);
	if (msg == DIE)
		printf("%ld %d died\n", time, philo->ind + 1);
	else if (msg == THINK)
		printf("%ld %d is thinking\n", time, philo->ind + 1);
	else if (msg == FORK)
		printf("%ld %d has taken a fork\n", time, philo->ind + 1);
	else if (msg == EAT)
		printf("%ld %d is eating\n", time, philo->ind + 1);
	else if (msg == SLEEP)
		printf("%ld %d is sleeping\n", time, philo->ind + 1);
	sem_post(philo->share->print_sem);
	philo->msg = NONE;
}
