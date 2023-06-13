/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/13 13:32:09 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philos;
	int		check;
	int		i;

	share = check_init_args(argc, argv);
	if (share == NULL)
		return (perror_n_return(EXIT_FAILURE));
	philos = init_philos(share);
	if (philos == NULL)
	{
		clear_share(share);
		return (perror_n_return(EXIT_FAILURE));
	}
	i = rev_iterator(thread_create(share, philos, &check), share->n_philo);
	while (i > -1)
	{
		check += pthread_join((philos + i)->thread, NULL);
		i = rev_iterator(i, share->n_philo);
	}
	if (share->n_eat > -1)
		check += pthread_join(share->monitoring, NULL);
	clear_all(share, philos);
	return (check);
}

int	thread_create(t_share *share, t_philo *philos, int *check)
{
	int	i;

	*check = 0;
	gettimeofday(&(share->t_start), NULL);
	i = (share->n_philo > 1) * 1;
	while (i < share->n_philo)
	{
		*check = pthread_create(&((philos + i)->thread), NULL,
				philo_routine, (philos + i));
		if (*check != 0)
			return (i);
		i = odd_even_iterator(i, share->n_philo);
	}
	if (share->n_eat > -1 && *check == 0)
		*check = pthread_create(&share->monitoring, NULL,
				monitoring_routine, philos);
	else if (*check != 0)
	{
		pthread_mutex_lock(&philos->share->all_alive_lock);
		philos->share->all_alive = ALL_DONE_EAT;
		pthread_mutex_unlock(&philos->share->all_alive_lock);
	}
	return (i);
}

int	odd_even_iterator(int i, int n_philo)
{
	int	result;

	if (i % 2 == 1)
	{
		result = i + 2;
		if (result >= n_philo)
			result = 0;
	}
	else
		result = i + 2;
	return (result);
}

int	rev_iterator(int i, int n_philo)
{
	int	result;

	if (i % 2 == 0)
	{
		result = i - 2;
		if (result < 0 && (n_philo % 2 == 0))
			result = n_philo - 1;
		else if (result < 0 && (n_philo % 2 == 1))
			result = n_philo - 2;
	}
	else
		result = i - 2;
	return (result);
}

void	clear_all(t_share *share, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < share->n_philo)
	{
		pthread_mutex_destroy(&(philos[i].pub_alive_lock));
		i++;
	}
	free(philos);
	clear_share(share);
}

void	clear_share(t_share *share)
{
	int	i;

	i = 0;
	free(share->forks);
	while (i < share->n_philo)
		pthread_mutex_destroy(share->fork_locks + i++);
	free(share->fork_locks);
	pthread_mutex_destroy(&share->all_alive_lock);
	free(share);
}
