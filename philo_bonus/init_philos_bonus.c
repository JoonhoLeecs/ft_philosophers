/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:27:56 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/21 07:57:24 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philo	*init_philos(t_share *share)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)malloc((share->n_philo) * sizeof(t_philo));
	if (philos == NULL)
		return (NULL);
	memset(philos, 0, sizeof(t_share) * share->n_philo);
	i = 0;
	while (i < share->n_philo)
	{
		philos[i].share = share;
		philos[i].ind = i;
		philos[i].n_eat = 0;
		philos[i].n_forks = 0;
		philos[i].status = THINKING;
		philos[i].alive = ALIVE;
		if (init_philo_sems(philos, i) != 0)
			return (NULL);
		i++;
	}
	return (philos);
}

int	init_philo_sems(t_philo *philos, int ind)
{
	philos[ind].last_eat_sem_name = get_last_eat_sem_name(ind);
	sem_unlink(philos[ind].last_eat_sem_name);
	philos[ind].last_eat_sem = sem_open(philos[ind].last_eat_sem_name, O_CREAT,
			0644, 1);
	if (philos[ind].last_eat_sem == SEM_FAILED)
	{
		free(philos[ind].last_eat_sem_name);
		while (--ind > -1)
		{
			sem_close(philos[ind].last_eat_sem);
			sem_unlink(philos[ind].last_eat_sem_name);
			free(philos[ind].last_eat_sem_name);
		}
		free(philos);
		return (1);
	}
	return (0);
}

char	*get_last_eat_sem_name(int i)
{
	int		len;
	int		ind;
	char	*result;

	len = get_n_digit(i) + 1 + 14;
	result = (char *)malloc(len * sizeof (char));
	if (result == NULL)
		exit (EXIT_FAILURE);
	memset(result, 0, len);
	ind = put_names(result, 0);
	philo_put_nbr((result + ind), i, len - 15);
	ind = put_names((result + ind + len - 15 + 1), 1);
	return (result);
}

int	put_names(char *str, int ind)
{
	char	*rest;
	int		j;

	if (ind == 0)
		rest = "/last_eat";
	else
		rest = ".sem";
	j = 0;
	while (rest[j])
	{
		*(str + j) = rest[j];
		j++;
	}
	*(str + j) = '\0';
	return (j);
}
