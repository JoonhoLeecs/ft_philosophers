/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_init_args_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:27:56 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 07:56:26 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	share->print_sem_name = "/print.sem";
	share->forks_sem_name = "/forks.sem";
	share->fulls_sem_name = "/fulls.sem";
	sem_unlink(share->print_sem_name);
	share->print_sem = sem_open(share->print_sem_name, O_CREAT,
			0644, 1);
	if (share->print_sem == SEM_FAILED)
	{
		printf("print sem failed\n");
		return (EXIT_FAILURE);
	}
	sem_unlink(share->forks_sem_name);
	share->forks_sem = sem_open(share->forks_sem_name, O_CREAT,
			0644, share->n_philo);
	if (share->forks_sem == SEM_FAILED)
	{
		printf("fork sem failed\n");
		sem_close(share->forks_sem);
		sem_unlink(share->forks_sem_name);
		return (EXIT_FAILURE);
	}
	sem_unlink(share->fulls_sem_name);
	if (share->n_eat > -1)
	{
		share->fulls_sem = sem_open(share->fulls_sem_name, O_CREAT,
				0644, share->n_philo);
		if (share->forks_sem == SEM_FAILED)
		{
			printf("fulls sem failed\n");
			sem_close(share->forks_sem);
			sem_unlink(share->forks_sem_name);
			sem_close(share->print_sem);
			sem_unlink(share->print_sem_name);
			return (EXIT_FAILURE);
		}
		i = 0;
		while (i < share->n_philo)
		{
			sem_wait(share->fulls_sem);
			i++;
		}
	}
	// share->all_alive = ALL_ALIVE;
	return (EXIT_SUCCESS);
}

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
		i++;
	}
	return (philos);
}
