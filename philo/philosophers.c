/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/09 22:02:46 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	check_leak(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philos;
	int		check;
	int		i;

	share = check_init_args(argc, argv);
	if (share == NULL)
		return (perror_n_return(EXIT_FAILURE));
	// test_print_share(share);
	philos = init_philos(share);
	if (philos == NULL)
	{
		free(share->forks);
		free(share->fork_locks);
		free(share);
		return (perror_n_return(EXIT_FAILURE));
	}
	gettimeofday(&(share->t_start), NULL);
	check = 0;
	i = 1;
	while (i < share->n_philo)
	{
		check = pthread_create(&((philos + i)->thread), NULL,
				philo_routine, (philos + i));
		if (check != 0)
			return (EXIT_FAILURE);
		i += 2;
	}
	i = 0;
	while (i < share->n_philo)
	{
		check = pthread_create(&((philos + i)->thread), NULL,
				philo_routine, (philos + i));
		if (check != 0)
			return (EXIT_FAILURE);
		i += 2;
	}
	i = 0;
	while (i < share->n_philo)
	{
		check = pthread_join((philos + i)->thread, NULL);
		if (check != 0)
			return (EXIT_FAILURE);
		i++;
	}
	// printf("thread of id %ld is created\n", thread->__sig);
	// check = simulation(share, philos);
	// atexit(check_leak);
	return (0);
}

t_philo	*init_philos(t_share *share)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)malloc((share->n_philo) * sizeof(t_philo));
	if (philos == NULL)
		return (NULL);
	i = 0;
	while (i < share->n_philo)
	{
		philos[i].share = share;
		philos[i].ind = i;
		philos[i].alive = 1;
		philos[i].n_eat = 0;
		philos[i].n_forks = 0;
		if (i % 2 == 0)
		{
			philos[i].first_fork = i;
			philos[i].second_fork = (i + 1) % share->n_philo;
		}
		else
		{
			philos[i].second_fork = i;
			philos[i].first_fork = (i + 1) % share->n_philo;
		}
		philos[i].status = TO_EAT;
		i++;
	}
	return (philos);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	void	(*philo_actions[3])(t_philo *);

	philo = (t_philo *)arg;
	philo_actions[0] = &philo_eat;
	philo_actions[1] = &philo_sleep;
	philo_actions[2] = &philo_think;
	gettimeofday(&(philo->t_last_eat), NULL);
	gettimeofday(&(philo->t_last_sleep), NULL);
	if (philo->ind % 2 == 0)
		usleep(philo_max(100, philo->share->n_philo * 2));
	while (philo->alive == 1)
	{
		philo_actions[(int)philo->status](philo);
	}
	return (NULL);
}
	// printf("last_eat:  %ld|%d sec\n", philo->t_last_eat.tv_sec, philo->t_last_eat.tv_usec);
	// printf("last_sleep:%ld|%d sec\n", philo->t_last_sleep.tv_sec, philo->t_last_sleep.tv_usec);

int	check_starvation(t_philo *philo, t_timeval time)
{
	if (philo->share->all_alive != 1)
	{
		put_back_forks(philo);
		philo->alive = 2;
	}
	else if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_die)
	{
		pthread_mutex_lock(&philo->share->print_lock);
		printf("%ld %d died\n",
			get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
		pthread_mutex_unlock(&philo->share->print_lock);
		pthread_mutex_lock(&philo->share->all_alive_lock);
		philo->share->all_alive = 0;
		pthread_mutex_unlock(&philo->share->all_alive_lock);
		put_back_forks(philo);
		philo->alive = 0;
	}
	return (philo->alive);
}

void	philo_eat(t_philo *philo)
{
	t_timeval	time;

	while (philo->alive == 1
		&& (philo->status == EATING || philo->status == TO_EAT))
	{
		gettimeofday(&time, NULL);
		if (check_starvation(philo, time) != 1)
			return ;
		if (philo->status == TO_EAT && philo->n_forks < 2)
		{
			take_forks(philo);
			continue ;
		}
		if (philo->status == TO_EAT && philo->n_forks == 2)
		{
			pthread_mutex_lock(&philo->share->print_lock);
			printf("%ld %d is eating\n",
				get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
			pthread_mutex_unlock(&philo->share->print_lock);
			philo->t_last_eat = time;
			philo->status = EATING;
			continue ;
		}
		else if (get_utime_diff(time, philo->t_last_eat) > philo->share->t_eat)
		{
			put_back_forks(philo);
			if (philo->share->n_eat > 0)
				philo->n_eat += 1;
			if (philo->n_eat == philo->share->n_eat)
				philo->alive = 2;
			philo->status = TO_SLEEP;
			return ;
		}
		usleep(T_UNIT);
	}
}

void	philo_sleep(t_philo *philo)
{
	t_timeval	time;

	while (philo->alive == 1
		&& (philo->status == SLEEPING || philo->status == TO_SLEEP))
	{
		gettimeofday(&time, NULL);
		if (check_starvation(philo, time) != 1)
			return ;
		if (philo->status == TO_SLEEP)
		{
			pthread_mutex_lock(&philo->share->print_lock);
			printf("%ld %d is sleeping\n",
				get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
			pthread_mutex_unlock(&philo->share->print_lock);
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
		usleep(T_UNIT);
	}
}

void	philo_think(t_philo *philo)
{
	t_timeval	time;

	if (philo->alive == 1
		&& philo->status == TO_THINK)
	{
		gettimeofday(&time, NULL);
		if (check_starvation(philo, time) != 1)
			return ;
		pthread_mutex_lock(&philo->share->print_lock);
		printf("%ld %d is thinking\n",
			get_mtime_diff(time, philo->share->t_start), philo->ind + 1);
		pthread_mutex_unlock(&philo->share->print_lock);
		philo->status = TO_EAT;
	}
}

	// the followings are test code for gettimeofday();
	// t_timeval	start;
	// t_timeval	end;

	// gettimeofday(&start, NULL);
	// gettimeofday(&end, NULL);
	// printf("start.tv_sec:%ld|tv_usec:%d\n", start.tv_sec, start.tv_usec);
	// printf("time interval in sec:%ld\n", end.tv_sec - start.tv_sec);
	// printf("time interval in usec:%d\n", end.tv_usec - start.tv_usec);

void test_print_share(t_share *share)
{
	printf("n_philo:%d\n", share->n_philo);
	printf("t_die:%ld\n", share->t_die);
	printf("t_eat:%ld\n", share->t_eat);
	printf("t_sleep:%ld\n", share->t_sleep);
	printf("n_eat:%d\n", share->n_eat);
}

void	take_forks(t_philo *philo)
{
	t_timeval	after_lock;

	if (philo->n_forks == 0)
	{
		pthread_mutex_lock(philo->share->fork_locks + philo->first_fork);
		gettimeofday(&after_lock, NULL);
		philo->n_forks += 1;
		if (check_starvation(philo, after_lock) != 1)
			return ;
		pthread_mutex_lock(&philo->share->print_lock);
		printf("%ld %d has taken a fork\n",
			get_mtime_diff(after_lock, philo->share->t_start), philo->ind + 1);
		pthread_mutex_unlock(&philo->share->print_lock);
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
		if (check_starvation(philo, after_lock) != 1)
			return ;
		pthread_mutex_lock(&philo->share->print_lock);
		printf("%ld %d has taken a fork\n",
			get_mtime_diff(after_lock, philo->share->t_start), philo->ind + 1);
		pthread_mutex_unlock(&philo->share->print_lock);
		*(philo->share->forks + philo->second_fork) = philo->ind;
	}
}

void	put_back_forks(t_philo *philo)
{
	if (philo->n_forks == 2)
	{
		*(philo->share->forks + philo->second_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->second_fork);
		philo->n_forks -= 1;
	}
	if (philo->n_forks == 1)
	{
		*(philo->share->forks + philo->first_fork) = 0;
		pthread_mutex_unlock(philo->share->fork_locks + philo->first_fork);
		philo->n_forks -= 1;
	}
}
