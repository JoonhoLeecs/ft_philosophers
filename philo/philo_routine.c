/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/12 21:27:29 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	void	(*philo_actions[3])(t_philo *);
	// int		unit_time;
	// t_timeval	time;

	philo = (t_philo *)arg;
	philo_actions[0] = &philo_eat;
	philo_actions[1] = &philo_sleep;
	philo_actions[2] = &philo_think;
	gettimeofday(&(philo->t_last_eat), NULL);
	philo->t_last_sleep = philo->t_last_eat;
	if (philo->ind % 2 == 0)
		usleep(philo_max(100, philo->share->n_philo * 5));
	while (philo->alive != DEAD)
	{
		// gettimeofday(&time, NULL);
		// unit_time = T_UNIT / 2;
		// if (check_starvation(philo, time) == DEAD)
		// 	continue ;
		// unit_time = refresh_unit_time(philo, time);
		philo_actions[(int)philo->status](philo);

		// usleep(unit_time);
	}
	return (NULL);
}
// printf("last_eat:  %ld|%d sec\n", philo->t_last_eat.tv_sec,
// 	philo->t_last_eat.tv_usec);
// printf("last_sleep:%ld|%d sec\n", philo->t_last_sleep.tv_sec,
// 	philo->t_last_sleep.tv_usec);

int	check_starvation(t_philo *philo, t_timeval time)
{
	// t_all_alive	all_alive;

	// pthread_mutex_lock(&philo->share->all_alive_lock);
	// all_alive = philo->share->all_alive;
	// pthread_mutex_unlock(&philo->share->all_alive_lock);
	// if (all_alive != ALL_ALIVE)
	// {
	// 	put_back_forks(philo);
	// 	philo->alive = DEAD;
	// }
	if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_die)
	{
		philo_printf(get_mtime_diff(time, philo->share->t_start),
			DIE, philo);
		// pthread_mutex_lock(&philo->share->print_lock);
		// printf("%ld %d died\n",
		// 	get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
		// pthread_mutex_unlock(&philo->share->print_lock);
		// pthread_mutex_lock(&philo->share->all_alive_lock);
		// philo->share->all_alive = ANY_DEAD;
		// pthread_mutex_unlock(&philo->share->all_alive_lock);
		put_back_forks(philo);
		philo->alive = DEAD;
		pthread_mutex_lock(&philo->pub_alive_lock);
		philo->pub_alive = DEAD;
		pthread_mutex_unlock(&philo->pub_alive_lock);
	}
	return (philo->alive);
}

void	philo_eat(t_philo *philo)
{
	t_timeval	time;
	int			unit_time;

	while (philo->alive != DEAD
		&& (philo->status == EATING || philo->status == TO_EAT))
	{
		gettimeofday(&time, NULL);
		unit_time = T_UNIT / 2;
		if (check_starvation(philo, time) == DEAD)
			return ;
		unit_time = refresh_unit_time(philo, time);
		if (philo->status == TO_EAT && philo->n_forks < 2)
		{
			take_forks(philo);
			continue ;
		}
		if (philo->status == TO_EAT && philo->n_forks == 2)
		{
			philo_printf(get_mtime_diff(time, philo->share->t_start),
				EAT, philo);
			// pthread_mutex_lock(&philo->share->print_lock);
			// gettimeofday(&time, NULL);
			// printf("%ld %d is eating\n",
			// 	get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
			// pthread_mutex_unlock(&philo->share->print_lock);
			philo->t_last_eat = time;
			philo->status = EATING;
			continue ;
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
			return ;
		}
		usleep(unit_time);
	}
}

void	philo_sleep(t_philo *philo)
{
	t_timeval	time;
	int			unit_time;

	while (philo->alive != DEAD
		&& (philo->status == SLEEPING || philo->status == TO_SLEEP))
	{
		gettimeofday(&time, NULL);
		unit_time = T_UNIT / 2;
		if (check_starvation(philo, time) == DEAD)
			return ;
		unit_time = refresh_unit_time(philo, time);
		if (philo->status == TO_SLEEP)
		{
			philo_printf(get_mtime_diff(time, philo->share->t_start),
				SLEEP, philo);
			// pthread_mutex_lock(&philo->share->print_lock);
			// gettimeofday(&time, NULL);
			// printf("%ld %d is sleeping\n",
			// 	get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
			// pthread_mutex_unlock(&philo->share->print_lock);
			philo->t_last_sleep = time;
			philo->status = SLEEPING;
			continue ;
		}
		else if (get_utime_diff(time, philo->t_last_sleep)
			> philo->share->t_sleep)
		{
			philo->status = TO_THINK;
			return ;
		}
		usleep(unit_time);
	}
}

void	philo_think(t_philo *philo)
{
	t_timeval	time;

	if (philo->alive != DEAD
		&& philo->status == TO_THINK)
	{
		gettimeofday(&time, NULL);
		if (check_starvation(philo, time) == DEAD)
			return ;
		philo_printf(get_mtime_diff(time, philo->share->t_start),
			THINK, philo);
		// pthread_mutex_lock(&philo->share->print_lock);
		// printf("%ld %d is thinking\n",
		// 	get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
		// pthread_mutex_unlock(&philo->share->print_lock);
		philo->status = TO_EAT;
	}
}

void	take_forks(t_philo *philo)
{
	t_timeval	after_lock;

	if (philo->n_forks == 0)
	{
		pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
		gettimeofday(&after_lock, NULL);
		philo->n_forks += 1;
		if (check_starvation(philo, after_lock) == DEAD)
			return ;
		philo_printf(get_mtime_diff(after_lock, philo->share->t_start),
			FORK, philo);
		// pthread_mutex_lock(&philo->share->print_lock);
		// printf("%ld %d has taken a fork\n",
		// 	get_mtime_diff(after_lock, philo->share->t_start), philo->ind + 1);
		// pthread_mutex_unlock(&philo->share->print_lock);
		*(philo->share->forks + philo->first_fork) = philo->ind;
	}
	if (philo->n_forks == 1 && philo->first_fork == philo->second_fork)
	{
		usleep(T_UNIT);
		return ;
	}
	else if (philo->n_forks == 1)
	{
		pthread_mutex_lock(philo->share->fork_locks + philo->second_fork);
		gettimeofday(&after_lock, NULL);
		philo->n_forks += 1;
		if (check_starvation(philo, after_lock) == DEAD)
			return ;
		philo_printf(get_mtime_diff(after_lock, philo->share->t_start),
			FORK, philo);
		// pthread_mutex_lock(&philo->share->print_lock);
		// printf("%ld %d has taken a fork\n",
		// 	get_mtime_diff(after_lock, philo->share->t_start), philo->ind + 1);
		// pthread_mutex_unlock(&philo->share->print_lock);
		*(philo->share->forks + philo->second_fork) = philo->ind;
	}
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
	// if (philo->n_forks == 2)
	// {
	// 	*(philo->share->forks + philo->second_fork) = 0;
	// 	pthread_mutex_unlock(philo->share->fork_locks + philo->second_fork);
	// 	philo->n_forks -= 1;
	// }
	// if (philo->n_forks == 1)

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
		// pthread_mutex_lock(&philo->share->print_lock);
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
		// pthread_mutex_unlock(&philo->share->print_lock);
	}
	if (all_alive != ALL_ALIVE)
	{
		put_back_forks(philo);
		philo->alive = DEAD;
	}
	pthread_mutex_unlock(&philo->share->all_alive_lock);
}

