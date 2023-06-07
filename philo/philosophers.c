/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/07 09:35:17 by joonhlee         ###   ########.fr       */
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

	share = check_init_args(argc, argv);
	if (share == NULL)
		return (perror_n_return(1));
	test_print_share(share);
	philos = init_philos(share);
	if (philos == NULL)
		return (EXIT_FAILURE);

	pthread_t	thread;

	check = pthread_create(&thread, NULL, philo_routine, philos + 1);
	if (check != 0)
		return (EXIT_FAILURE);
	else
		printf("thread of id %ld is created\n", thread);


	// check = simulation(share, philos);
	// atexit(check_leak);
	return (0);
}

t_philo	*init_philos(t_share *share)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)malloc((share->n_philo + 1) * sizeof(t_philo));
	if (philos == NULL)
		return (NULL);
	i = 1;
	while (i < share->n_philo + 1)
	{
		philos[i].share = share;
		philos[i].alive = 1;
		philos[i].ind = i + 1;
		philos[i].n_eat = 0;
		philos[i].t_last_eat = -1;
		philos[i].t_last_sleep = -1;
		i++;
	}
	return (philos);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		check;

	philo = (t_philo *)arg;
	philo->t_last_eat = get_utime();
	while (philo->alive == 1)
		philo->share->philo_actions[philo->status](philo);
}

void	philo_eat(t_philo *philo)
{
	int	check;

	check_starvation(philo);
	while(philo->alive == 1)
	{

		usleep(T_UNIT);
		check_starvation(philo);
	}

}
void	philo_sleep(t_philo *philo)
{

}
void	philo_think(t_philo *philo)
{

}

	// the followings are test code for gettimeofday();
	// t_timeval	start;
	// t_timeval	end;

	// gettimeofday(&start, NULL);
	// gettimeofday(&end, NULL);
	// printf("start.tv_sec:%ld|tv_usec:%d\n", start.tv_sec, start.tv_usec);
	// printf("time interval in sec:%ld\n", end.tv_sec - start.tv_sec);
	// printf("time interval in usec:%d\n", end.tv_usec - start.tv_usec);

void test_print_share(t_share *share)
{
	printf("n_philo:%d\n", share->n_philo);
	printf("t_die:%d\n", share->t_die);
	printf("t_eat:%d\n", share->t_eat);
	printf("t_sleep:%d\n", share->t_sleep);
	printf("n_eat:%d\n", share->n_eat);
}
