/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:30 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/21 08:42:31 by joonhlee         ###   ########.fr       */
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

# define T_UNIT 31
# define T_OFFSET 100
# define INT_MAX 2147483647

typedef enum e_status
{
	TO_EAT = 0,
	TO_SLEEP,
	TO_THINK,
	EATING,
	SLEEPING,
	THINKING,
}	t_status;

typedef enum e_alive
{
	DEAD = 0,
	ALIVE,
	DONE_EAT,
}	t_alive;

typedef enum e_all_alive
{
	ALL_ALIVE = 0,
	ANY_DEAD,
	ALL_DONE_EAT,
}	t_all_alive;

typedef enum e_msg
{
	NONE = 0,
	DIE,
	THINK,
	FORK,
	EAT,
	SLEEP,
	SKIP,
}	t_msg;

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
	t_status	status;
	t_msg		msg;
	t_alive		alive;
	t_alive		pub_alive;
	t_mutex		pub_alive_lock;
	t_timeval	t_last_eat;
	t_timeval	t_last_sleep;
}				t_philo;

typedef struct s_monitor_env
{
	t_alive		pub_alive;
	int			i;
	int			check;
	t_timeval	t_last_eat;
}			t_monitor_env;

t_share	*check_init_args(int argc, char **argv);
int		save_args(t_share *share, int *n, int i);
int		init_share_mutex(t_share *share);
t_philo	*init_philos(t_share *share);
void	init_philo_var(t_philo *philos, int i, t_share *share);
t_philo	*abort_init_philos(t_philo *philos, int i);
int		thread_create(t_share *share, t_philo *philos, int *check);
int		odd_even_iterator(int i, int n_philo);
int		rev_iterator(int i, int n_philo);

void	*philo_routine(void *arg);
void	routine_init(t_philo *philo, \
		int (*actions[])(t_philo *, t_timeval time));
int		check_starvation(t_philo *philo, t_timeval time);
int		philo_eat(t_philo *philo, t_timeval time);
int		philo_sleep(t_philo *philo, t_timeval time);
int		philo_think(t_philo *philo, t_timeval time);
int		take_forks(t_philo *philo, t_timeval time);
int		only_philo_take_forks(t_philo *philo, t_timeval time);
void	check_n_grab_forks(t_philo *philo);
void	put_back_forks(t_philo *philo);
int		refresh_unit_time(t_philo *philo, t_timeval time);
int		refresh_unit_time2(t_philo *philo, t_timeval time);
void	philo_printf(t_msg msg, t_philo *philo);

void	*monitoring_routine(void *arg);
void	init_monitoring(t_monitor_env *env, t_philo *philos);
void	check_all_done(t_monitor_env env, t_philo *philos);

int		philo_atoi(const char *str);
int		perror_n_return(int exit_status);
long	get_utime_diff(t_timeval time, t_timeval ref);
long	get_mtime_diff(t_timeval time, t_timeval ref);
int		philo_max(int a, int b);
long	philo_min(long a, long b);
int		get_n_digit(long nbr);
void	philo_put_nbr(char *str, long nbr, int n_digits);
int		philo_put_rest(char *str, t_msg msg);
void	philo_print(long time, int id, t_msg msg);

void	clear_all(t_share *share, t_philo *philos);
void	clear_share(t_share *share);
int		clear_share_perror_return(t_share *share, int exit_status);

#endif
