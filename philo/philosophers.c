/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/19 09:33:15 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philos;
	int		check;
	int		i;

	share = check_init_args(argc, argv);
	if (share == NULL)
		return (perror_n_return(EXIT_FAILURE));
	philos = init_philos(share);
	if (philos == NULL)
		return (clear_share_perror_return(share, EXIT_FAILURE));
	i = rev_iterator(thread_create(share, philos, &check), share->n_philo);
	if (check == 0)
		check += pthread_join(share->monitoring, NULL);
	while (i > -1)
	{
		check += pthread_join((philos + i)->thread, NULL);
		i = rev_iterator(i, share->n_philo);
	}
	clear_all(share, philos);
	return (check);
}

int	thread_create(t_share *share, t_philo *philos, int *check)
{
	int	i;
	int	j;

	*check = 0;
	pthread_mutex_lock(&share->all_alive_lock);
	i = (share->n_philo > 1) * 1;
	while (i < share->n_philo)
	{
		*check = pthread_create(&((philos + i)->thread), NULL,
				philo_routine, (philos + i));
		if (*check != 0)
			break ;
		i = odd_even_iterator(i, share->n_philo);
	}
	if (*check == 0)
		j = pthread_create(&share->monitoring, NULL,
				monitoring_routine, philos);
	gettimeofday(&(share->t_start), NULL);
	if (*check != 0 || j != 0)
	{
		philos->share->all_alive = ALL_DONE_EAT;
		*check += j;
	}
	pthread_mutex_unlock(&share->all_alive_lock);
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
