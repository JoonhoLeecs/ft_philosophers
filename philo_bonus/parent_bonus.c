/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:08:22 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/10 18:51:51 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	parent(t_share *share, t_philo *philos, int ind)
{
	int		i;
	int		status;
	int		pid;

	usleep(philo_max(100, philos->share->n_philo * 3));
	// check = 0;
	// while (check < philos->share->n_philo)
	// {
	// 	check = 0;
	i = 0;
	while (i < philos->share->n_philo)
	{
		pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 0)
				break ;
			// else if (WEXITSTATUS(status) == 2)
		}

		i++;
	}
	i = ind;
	// 	usleep(1 * T_UNIT);
	// }
	// pthread_mutex_lock(&philos->share->all_alive_lock);
	philos->share->all_alive = ALL_DONE_EAT;
	// pthread_mutex_unlock(&philos->share->all_alive_lock);
	clear_all(share, philos);
	return (EXIT_SUCCESS);
}
