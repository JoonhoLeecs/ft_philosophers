/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:27:56 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/22 09:43:01 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philo	*init_philos(t_share *share)
{
	t_philo	*philos;
	int		i;
	int		check;

	philos = (t_philo *)malloc((share->n_philo) * sizeof(t_philo));
	if (philos == NULL)
		return (NULL);
	memset(philos, 0, sizeof(t_philo) * share->n_philo);
	i = 0;
	check = 0;
	while (i < share->n_philo && check == 0)
	{
		init_philo_var(philos, i, share);
		check = pthread_mutex_init(&(philos[i].pub_alive_lock), NULL);
		if (check == 0)
			check = pthread_mutex_init(&(philos[i].pub_t_last_eat_lock), NULL);
		i++;
	}
	if (check != 0)
		return (abort_init_philos(philos, i));
	return (philos);
}

void	init_philo_var(t_philo *philos, int i, t_share *share)
{
	philos[i].share = share;
	philos[i].ind = i;
	philos[i].n_eat = 0;
	philos[i].n_forks = 0;
	philos[i].first_fork = i;
	philos[i].second_fork = (i + 1) % share->n_philo;
	philos[i].status = THINKING;
	philos[i].alive = ALIVE;
	philos[i].pub_alive = ALIVE;
}

t_philo	*abort_init_philos(t_philo *philos, int i)
{
	while (--i > -1)
	{
		pthread_mutex_destroy(&(philos[i].pub_alive_lock));
		pthread_mutex_destroy(&(philos[i].pub_t_last_eat_lock));
	}
	free(philos);
	return (NULL);
}
