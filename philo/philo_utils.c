/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 08:58:21 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/19 08:38:40 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

int	perror_n_return(int exit_status)
{
	printf("Error: invalid arguement\n");
	return (exit_status);
}

long	get_utime_diff(t_timeval time, t_timeval ref)
{
	long	diff;

	diff = (time.tv_sec - ref.tv_sec) * 1000000 + (time.tv_usec - ref.tv_usec);
	return (diff);
}

long	get_mtime_diff(t_timeval time, t_timeval ref)
{
	long	diff;

	diff = (time.tv_sec - ref.tv_sec) * 1000
		+ (time.tv_usec - ref.tv_usec) / 1000;
	return (diff);
}
