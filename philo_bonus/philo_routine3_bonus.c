/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine3_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 13:48:22 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_printf(long time, t_msg msg, t_philo *philo)
{
	sem_wait(philo->share->print_sem);
	if (msg == DIE)
		printf("%ld %d died\n", time, philo->ind + 1);
	else if (msg == THINK)
		printf("%ld %d is thinking\n", time, philo->ind + 1);
	else if (msg == FORK)
		printf("%ld %d has taken a fork\n", time, philo->ind + 1);
	else if (msg == EAT)
		printf("%ld %d is eating\n", time, philo->ind + 1);
	else if (msg == SLEEP)
		printf("%ld %d is sleeping\n", time, philo->ind + 1);
	sem_post(philo->share->print_sem);
	philo->msg = NONE;
}
