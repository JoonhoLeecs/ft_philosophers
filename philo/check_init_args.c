/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_init_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:27:56 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/10 10:59:38 by joonhlee         ###   ########.fr       */
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
	if (save_args(share, n, i))
	{
		free(share);
		return (NULL);
	}
	return (share);
}

int	save_args(t_share *share, int *n, int i)
{
	int	j;

	share->n_philo = n[0];
	share->t_die = n[1] * 1000;
	share->t_eat = n[2] * 1000;
	share->t_sleep = n[3] * 1000;
	share->n_eat = -1;
	if (i > 5)
		share->n_eat = n[4];
	share->forks = (int *) malloc(sizeof (int) * share->n_philo);
	if (share->forks == NULL)
		return (EXIT_FAILURE);
	share->fork_locks = (t_mutex *) malloc(sizeof (t_mutex) * share->n_philo);
	if (share->fork_locks == NULL)
	{
		free(share->forks);
		share->forks = NULL;
		return (EXIT_FAILURE);
	}
	j = 0;
	while (j < share->n_philo)
		pthread_mutex_init(share->fork_locks + j++, NULL);
	pthread_mutex_init(&(share->print_lock), NULL);
	pthread_mutex_init(&(share->all_alive_lock), NULL);
	share->all_alive = ALL_ALIVE;
	return (EXIT_SUCCESS);
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
		philos[i].alive = ALIVE;
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
		// pthread_mutex_init(&(philos[i].alive_lock), NULL);
		i++;
	}
	return (philos);
}
