/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine3_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/21 08:42:45 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_printf(t_msg msg, t_philo *philo)
{
	t_timeval	print_time;

	sem_wait(philo->share->print_sem);
	gettimeofday(&print_time, NULL);
	philo_print(get_mtime_diff(print_time, philo->share->t_start),
		philo->ind + 1, msg);
	if (msg == EAT)
	{
		philo->t_last_eat = print_time;
		sem_wait(philo->last_eat_sem);
		philo->pub_t_last_eat = print_time;
		sem_post(philo->last_eat_sem);
	}
	else if (msg == SLEEP)
		philo->t_last_sleep = print_time;
	if (msg != DIE)
		sem_post(philo->share->print_sem);
	philo->msg = NONE;
}
