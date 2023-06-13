/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_init_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:27:56 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/13 19:05:27 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_share	*check_init_args(int argc, char **argv)
{
	int		i;
	int		n[5];
	t_share	*share;

	if (argc != 5 && argc != 6)
		return (NULL);
	i = 1;
	while (argv[i])
	{
		n[i - 1] = philo_atoi(argv[i]);
		if (n[i - 1] <= 0)
			return (NULL);
		i++;
	}
	share = (t_share *) malloc(sizeof(t_share));
	if (share == NULL)
		return (NULL);
	memset(share, 0, sizeof(t_share));
	if (save_args(share, n, i))
	{
		free(share);
		return (NULL);
	}
	return (share);
}

int	save_args(t_share *share, int *n, int i)
{
	share->n_philo = n[0];
	share->t_die = n[1] * 1000;
	share->t_eat = n[2] * 1000;
	share->t_sleep = n[3] * 1000;
	share->n_eat = -1;
	if (i > 5)
		share->n_eat = n[4];
	share->all_alive = ALL_ALIVE;
	share->forks = (int *) malloc(sizeof (int) * share->n_philo);
	if (share->forks == NULL)
		return (EXIT_FAILURE);
	share->fork_locks = (t_mutex *) malloc(sizeof (t_mutex) * share->n_philo);
	if (share->fork_locks == NULL)
	{
		free(share->forks);
		return (EXIT_FAILURE);
	}
	return (init_share_mutex(share));
}

int	init_share_mutex(t_share *share)
{
	int	j;
	int	check;

	check = 0;
	j = 0;
	while (j < share->n_philo && check == 0)
		check = pthread_mutex_init(share->fork_locks + j++, NULL);
	if (check != 0)
	{
		free(share->forks);
		free(share->fork_locks);
		while (--j >= 0)
			pthread_mutex_destroy(share->fork_locks + j);
		return (EXIT_FAILURE);
	}
	check = pthread_mutex_init(&share->all_alive_lock, NULL);
	if (check != 0)
	{
		free(share->forks);
		free(share->fork_locks);
		while (--j >= 0)
			pthread_mutex_destroy(share->fork_locks + j);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

t_philo	*init_philos(t_share *share)
{
	t_philo	*philos;
	int		i;
	int		check;

	philos = (t_philo *)malloc((share->n_philo) * sizeof(t_philo));
	if (philos == NULL)
		return (NULL);
	memset(philos, 0, sizeof(t_share) * share->n_philo);
	i = 0;
	check = 0;
	while (i < share->n_philo && check == 0)
	{
		init_philo_var(philos, i, share);
		check = pthread_mutex_init(&(philos[i].pub_alive_lock), NULL);
		i++;
	}
	printf("done philo init:%d\n", check);
	if (check != 0)
	{
		while (--i > -1)
			pthread_mutex_destroy(&(philos[i].pub_alive_lock));
		free(philos);
		return (NULL);
	}
	return (philos);
}

void	init_philo_var(t_philo *philos, int i, t_share *share)
{
	philos[i].share = share;
	philos[i].ind = i;
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
	philos[i].status = THINKING;
	philos[i].alive = ALIVE;
	philos[i].pub_alive = ALIVE;
}
