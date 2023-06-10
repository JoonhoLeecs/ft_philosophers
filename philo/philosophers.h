/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:30 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/10 11:31:50 by joonhlee         ###   ########.fr       */
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

# define T_UNIT 17
# define T_OFFSET 200

typedef enum e_status
{
	TO_EAT = 0,
	TO_SLEEP,
	TO_THINK,
	EATING,
	SLEEPING,
}	t_status;

typedef enum e_alive
{
	DEAD = 0,
	ALIVE,
	DONE_EAT,
}	t_alive;

typedef enum e_all_alive
{
	ALL_DONE_EAT = -1,
	ANY_DEAD,
	ALL_ALIVE,
}	t_all_alive;

typedef struct timeval	t_timeval;
typedef pthread_mutex_t	t_mutex;
typedef pthread_t		t_pthread;

typedef struct s_share
{
	int			n_philo;
	long		t_die;
	long		t_eat;
	long		t_sleep;
	int			n_eat;
	t_timeval	t_start;
	int			*forks;
	t_mutex		*fork_locks;
	t_mutex		print_lock;
	t_all_alive	all_alive;
	t_mutex		all_alive_lock;
	t_pthread	monitoring;
}				t_share;

typedef struct s_philo
{
	t_share		*share;
	t_pthread	thread;
	int			ind;
	int			n_eat;
	int			n_forks;
	int			first_fork;
	int			second_fork;
	t_alive		alive;
	// t_mutex		alive_lock;
	t_status	status;
	t_timeval	t_last_eat;
	t_timeval	t_last_sleep;
}				t_philo;

t_share	*check_init_args(int argc, char **argv);
int		save_args(t_share *share, int *n, int i);
t_philo	*init_philos(t_share *share);

void	*philo_routine(void *arg);
int		check_starvation(t_philo *philo, t_timeval time);
void	philo_eat(t_philo *philo);
void	take_forks(t_philo *philo);
void	put_back_forks(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
int refresh_unit_time(t_philo *philo, t_timeval time);

void	*monitoring_routine(void *arg);

int		philo_atoi(const char *str);
int		perror_n_return(int exit_status);
long	get_utime_diff(t_timeval time, t_timeval ref);
long	get_mtime_diff(t_timeval time, t_timeval ref);
int		philo_max(int a, int b);
long		philo_min(long a, long b);

void	test_print_share(t_share *share);

#endif
