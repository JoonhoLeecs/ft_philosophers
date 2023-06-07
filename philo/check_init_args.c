/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_init_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:27:56 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/07 09:16:31 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		if (n[i - 1] < 0)
			return (NULL);
		i++;
	}
	share = (t_share *) malloc(sizeof(t_share));
	if (share == NULL)
		return (NULL);
	save_args(share, n, i);
	return (share);
}

void	save_args(t_share *share, int *n, int i)
{
	share->n_philo = n[0];
	share->t_die = n[1];
	share->t_eat = n[2];
	share->t_sleep = n[3];
	if (i > 5)
		share->n_eat = n[4];
	else
		share->n_eat = -1;
	share->t_start = get_utime();
	share->philo_actions[0] = &philo_eat;
	share->philo_actions[1] = &philo_sleep;
	share->philo_actions[2] = &philo_think;
}
