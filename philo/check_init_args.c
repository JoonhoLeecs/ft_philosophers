/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_init_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:27:56 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/09 15:07:45 by joonhlee         ###   ########.fr       */
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
	if (i > 5)
		share->n_eat = n[4];
	else
		share->n_eat = -1;
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
	share->all_alive = 1;
	return (EXIT_SUCCESS);
}

	// share->print_locks = (t_mutex *) malloc(sizeof (t_mutex));
	// if (share->print_locks == NULL)
	// {
	// 	free(share->forks);
	// 	share->forks = NULL;
	// 	free(share->fork_locks);
	// 	share->fork_locks = NULL;
	// 	return (EXIT_FAILURE);
	// }
	// share->exit_locks = (t_mutex *) malloc(sizeof (t_mutex) * share->n_philo);
	// if (share->exit_locks == NULL)
	// {
	// 	free(share->forks);
	// 	share->forks = NULL;
	// 	free(share->fork_locks);
	// 	share->fork_locks = NULL;
	// 	free(share->print_locks);
	// 	share->print_locks = NULL;
	// 	return (EXIT_FAILURE);
	// }
