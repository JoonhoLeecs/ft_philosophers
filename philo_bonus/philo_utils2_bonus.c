/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:58:21 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/20 17:25:29 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		sem_close((philos + i)->last_eat_sem);
		sem_unlink((philos + i)->last_eat_sem_name);
		free((philos + i)->last_eat_sem_name);
		i++;
	}
	free(philos);
	clear_share(share);
}

void	clear_share(t_share *share)
{
	sem_close(share->forks_sem);
	sem_unlink(share->forks_sem_name);
	sem_close(share->print_sem);
	sem_unlink(share->print_sem_name);
	if (share->n_eat > -1)
	{
		sem_close(share->fulls_sem);
		sem_unlink(share->fulls_sem_name);
	}
	free(share);
}
