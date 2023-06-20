/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine3_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:33 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/20 16:46:06 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_printf(long time, t_msg msg, t_philo *philo)
{
	sem_wait(philo->share->print_sem);
	philo_print(time, philo->ind + 1, msg);
	if (msg != DIE)
		sem_post(philo->share->print_sem);
	philo->msg = NONE;
}
