/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 13:46:27 by joonhlee         ###   ########.fr       */
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
	if (philo->ind == 0)
	{
		philo->have_two_forks = 0;
		philo->done_check_forks = 0;
		if (pthread_mutex_init(&philo->two_forks_lock, NULL) != 0)
			exit (EXIT_FAILURE);
		if (pthread_create(&philo->forks_check, NULL, check_forks, philo) != 0)
			exit (EXIT_FAILURE);
	}
}

void	*check_forks(void *arg)
{
	t_philo		*philo;
	int			check;
	t_timeval	time;

	philo = (t_philo *)arg;
	usleep(philo->share->t_eat / 2);
	pthread_mutex_lock(&philo->two_forks_lock);
	check = philo->have_two_forks;
	pthread_mutex_unlock(&philo->two_forks_lock);
	if (check == 1)
	{
		gettimeofday(&time, NULL);
		usleep(philo->share->t_die - get_utime_diff(time, philo->t_last_eat));
		gettimeofday(&time, NULL);
		if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_die)
		{
			printf("%ld %d died\n", get_mtime_diff(time, philo->share->t_start),
				philo->ind + 1);
			kill (philo->pid, SIGINT);
		}
	}
	return (NULL);
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
