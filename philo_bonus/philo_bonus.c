/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 12:13:16 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philos;
	int		pid;
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
	gettimeofday(&(share->t_start), NULL);
	i = (share->n_philo > 1) * 1;
	while (i < share->n_philo)
	{
		pid = fork();
		if (pid == -1)
			break ;
		else if (pid == 0)
			return (philo_routine(philos + i));
		else
		{
			(philos + i)->pid = pid;
			i = odd_even_iterator(i, share->n_philo);
		}
	}
	return (parent(share, philos, i));
}

void	clear_all(t_share *share, t_philo *philos)
{
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
