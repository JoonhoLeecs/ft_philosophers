/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:58:21 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 13:18:27 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_max(int a, int b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

long	philo_min(long a, long b)
{
	if (a > b)
		return (b);
	else
		return (a);
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
