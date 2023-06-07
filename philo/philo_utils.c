/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:58:21 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/07 08:19:42 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <limits.h>

int	philo_atoi(const char *str)
{
	int		i;
	int		sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		sign = 44 - str[i++];
	if (sign < 0)
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign > 0 && ((nbr > INT_MAX / 10) || (nbr == INT_MAX / 10
					&& str[i] - '0' > INT_MAX % 10)))
			return (-1);
		nbr = nbr * 10 + sign * (str[i++] - '0');
	}
	if (str[i])
		return (-1);
	return ((int)nbr);
}

// return timeval.tv_usec as positive int value on success,
// or -1 on fail to call gettimeofday
int	get_utime(void)
{
	t_timeval	time;
	int			check;

	check = gettimeofday(&time, NULL);
	if (check == -1)
		return (-1);
	return (time.tv_usec);
}

int	perror_n_return(int exit_status)
{
	write(STDERR_FILENO, "Error: invalid arguement\n", 26);
	return (exit_status);
}
