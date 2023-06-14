/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_print_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 07:08:21 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 07:22:54 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	get_n_digit(long nbr)
{
	int	n_digits;

	n_digits = 0;
	while (nbr / 10 != 0)
	{
		nbr /= nbr;
		n_digits++;
	}
	return (n_digits);
}

void	philo_put_nbr(char *str, long nbr, int n_digits)
{
	while (n_digits > -1)
	{
		*(str + n_digits--) = nbr % 10 + '0';
		nbr /= 10;
	}
}

int	philo_put_rest(char *str, t_msg msg)
{
	char	*rest;
	int		j;

	if (msg == DIE)
		rest = "died\n";
	else if (msg == THINK)
		rest = "is thinking\n";
	else if (msg == FORK)
		rest = "has taken a fork\n";
	else if (msg == EAT)
		rest = "is eating\n";
	else if (msg == SLEEP)
		rest = "is sleeping\n";
	j = 0;
	while (rest[j])
	{
		*(str + j) = rest[j];
		j++;
	}
	*(str + j) = '\0';
	return (j);
}

void	philo_print(long time, int id, t_msg msg)
{
	char	res[52];
	int		n_digits;
	int		ind;
	int		j;
	char	*rest;

	n_digits = get_n_digit(time);
	philo_put_nbr(res, time, n_digits);
	ind = n_digits + 1;
	res[ind++] = ' ';
	n_digits = get_n_digit(id);
	philo_put_nbr(res + ind, time, n_digits);
	ind += n_digits + 1;
	res[ind++] = ' ';
	ind += philo_put_rest(res + ind, msg);
	write(STDOUT_FILENO, res, ind);
}
