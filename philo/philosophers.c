/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/10 15:17:24 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	check_leak(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philos;
	int		check;
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
	check = 0;
	i = 1;
	while (i < share->n_philo)
	{
		check = pthread_create(&((philos + i)->thread), NULL,
				philo_routine, (philos + i));
		if (check != 0)
			return (EXIT_FAILURE);
		i += 2;
	}
	i = 0;
	while (i < share->n_philo)
	{
		check = pthread_create(&((philos + i)->thread), NULL,
				philo_routine, (philos + i));
		if (check != 0)
			return (EXIT_FAILURE);
		i += 2;
	}
	// i = 0;
	// while (i < share->n_philo)
	// {
	// 	check = pthread_create(&((philos + i)->thread), NULL,
	// 			philo_routine, (philos + i));
	// 	if (check != 0)
	// 		return (EXIT_FAILURE);
	// 	i += 1;
	// }
	if (share->n_eat > -1)
		check = pthread_create(&share->monitoring, NULL,
				monitoring_routine, philos);
	i = 0;
	while (i < share->n_philo)
	{
		check = pthread_join((philos + i)->thread, NULL);
		if (check != 0)
			return (EXIT_FAILURE);
		i++;
	}
	if (share->n_eat > -1)
		check = pthread_join(share->monitoring, NULL);
	clear_all(share, philos);
	return (EXIT_SUCCESS);
}
	// the followings are test code for gettimeofday();
	// t_timeval	start;
	// t_timeval	end;

	// gettimeofday(&start, NULL);
	// gettimeofday(&end, NULL);
	// printf("start.tv_sec:%ld|tv_usec:%d\n", start.tv_sec, start.tv_usec);
	// printf("time interval in sec:%ld\n", end.tv_sec - start.tv_sec);
	// printf("time interval in usec:%d\n", end.tv_usec - start.tv_usec);

void	clear_all(t_share *share, t_philo *philos)
{
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
	pthread_mutex_destroy(&share->print_lock);
	pthread_mutex_destroy(&share->all_alive_lock);
	free(share);
}

void test_print_share(t_share *share)
{
	printf("n_philo:%d\n", share->n_philo);
	printf("t_die:%ld\n", share->t_die);
	printf("t_eat:%ld\n", share->t_eat);
	printf("t_sleep:%ld\n", share->t_sleep);
	printf("n_eat:%d\n", share->n_eat);
}
