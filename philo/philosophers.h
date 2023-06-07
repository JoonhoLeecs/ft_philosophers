/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:30 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/07 09:40:09 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

# include <stdio.h>
// need to check headers above after compeletion

# define T_UNIT 200

typedef enum e_status
{
	TO_EAT = 0,
	TO_SLEEP,
	TO_THINK,
	EATING,
	SLEEPING,
} t_status;

typedef struct s_share
{
	int		n_philo;
	int		t_die;
	int		t_eat;
	int		t_sleep;
	int		n_eat;
	int		t_start;
	void	(*philo_actions[3])(t_philo *);

	// int	print;
	// struct s_philo	*ready;
	// struct s_philo	*fork_top;
	// struct s_philo	*fork_tail;
	// struct s_philo	*print_top;
	// struct s_philo	*print_tail;

}		t_share;

typedef struct s_philo
{
	t_share	*share;
	int		ind;
	int		alive;
	int		n_eat;
	int		status;
	int		t_last_eat;
	int		t_last_sleep;
	// struct s_philo	*next;
	// struct s_philo	*prev;
}		t_philo;

typedef struct timeval	t_timeval;

t_share	*check_init_args(int argc, char **argv);
int		philo_atoi(const char *str);
void	save_args(t_share *share, int *n, int i);

t_philo	*init_philos(t_share *share);
void	*philo_routine(void *arg);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

	int get_utime(void);
int		perror_n_return(int exit_status);

void test_print_share(t_share *share);

#endif
