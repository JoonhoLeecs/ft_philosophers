/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:08:22 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 13:40:05 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	parent(t_share *share, t_philo *philos, int ind)
{
	int		i;
	int		status;
	int		pid;

	if (ind < share->n_philo)
	{
		kill(0, SIGINT);
		clear_all(share, philos);
		return (EXIT_FAILURE);
	}
	usleep(philo_max(T_OFFSET, philos->share->n_philo * 5) + T_OFFSET);
	if (share->n_eat > -1)
		init_monitor(share, philos);
	i = (share->n_eat > -1) * -1;
	while (i < philos->share->n_philo)
	{
		pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			if (WEXITSTATUS(status) == 3 || WEXITSTATUS(status) == 1)
				break ;
		i++;
	}
	kill(0, SIGINT);
	clear_all(share, philos);
	return (EXIT_SUCCESS);
}

void	init_monitor(t_share *share, t_philo *philos)
{
	int	i;

	share->pid_monitor = fork();
	if (share->pid_monitor == -1)
	{
		clear_all(share, philos);
		exit (EXIT_FAILURE);
	}
	else if (share->pid_monitor == 0)
	{
		i = 0;
		while (i < share->n_philo)
		{
			sem_wait(share->fulls_sem);
			i++;
		}
		sem_close(share->forks_sem);
		sem_close(share->print_sem);
		if (share->n_eat > -1)
			sem_close(share->fulls_sem);
		exit(SIGQUIT);
	}
	else
		return ;
}
