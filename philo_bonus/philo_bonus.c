/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/10 17:06:04 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	check_leak(void)
{
	system("leaks philo_bonus");
}

int	main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philos;
	int		pid;
	int		i;

	share = check_init_args(argc, argv);
	if (share == NULL)
		return (perror_n_return(EXIT_FAILURE));
	// test_print_share(share);
	philos = init_philos(share);
	if (philos == NULL)
	{
		clear_share(share);
		return (perror_n_return(EXIT_FAILURE));
	}
	gettimeofday(&(share->t_start), NULL);
	i = 1;
	while (i < share->n_philo)
	{
		pid = fork();
		if (pid == -1)
			return (EXIT_FAILURE);
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
	// int	i;

	// i = 0;
	// free(share->forks);
	// while (i < share->n_philo)
	// 	pthread_mutex_destroy(share->fork_locks + i++);
	// free(share->fork_locks);
	// pthread_mutex_destroy(&share->print_lock);
	// pthread_mutex_destroy(&share->all_alive_lock);

void test_print_share(t_share *share)
{
	printf("n_philo:%d\n", share->n_philo);
	printf("t_die:%ld\n", share->t_die);
	printf("t_eat:%ld\n", share->t_eat);
	printf("t_sleep:%ld\n", share->t_sleep);
	printf("n_eat:%d\n", share->n_eat);
}
