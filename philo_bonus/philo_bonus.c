/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/21 09:30:29 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philos;
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
	i = fork_child_philos(share, philos);
	return (parent(share, philos, i));
}

int	fork_child_philos(t_share *share, t_philo *philos)
{
	int	pid;
	int	i;

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
