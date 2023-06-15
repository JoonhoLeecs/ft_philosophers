/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 07:40:30 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/15 15:45:48 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <signal.h>
# include <pthread.h>

# define T_UNIT 31
# define T_OFFSET 200
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
	ALL_DONE_EAT = -1,
	ANY_DEAD,
	ALL_ALIVE,
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
typedef sem_t			t_sem;
typedef pthread_t		t_pthread;
typedef pthread_mutex_t	t_mutex;

typedef struct s_share
{
	int			n_philo;
	long		t_die;
	long		t_eat;
	long		t_sleep;
	int			n_eat;
	t_timeval	t_start;
	char		*forks_sem_name;
	t_sem		*forks_sem;
	char		*print_sem_name;
	t_sem		*print_sem;
	char		*fulls_sem_name;
	t_sem		*fulls_sem;
	int			pid_monitor;
}				t_share;

typedef struct s_philo
{
	t_share		*share;
	int			pid;
	int			ind;
	int			n_eat;
	int			n_forks;
	t_status	status;
	t_msg		msg;
	t_alive		alive;
	t_timeval	t_last_eat;
	t_timeval	t_last_sleep;
	t_pthread	forks_check;
	t_mutex		two_forks_lock;
	int			have_two_forks;
	int			done_check_forks;
}				t_philo;

t_share	*check_init_args(int argc, char **argv);
int		save_args(t_share *share, int *n, int i);
int		init_sems(t_share *share);
int		init_fulls_sem(t_share *share);
t_philo	*init_philos(t_share *share);
int		fork_child_philos(t_share *share, t_philo *philos);
int		odd_even_iterator(int i, int n_philo);
int		rev_iterator(int i, int n_philo);

int		philo_routine(t_philo *philo);
void	routine_init(t_philo *philo,
			int (*actions[])(t_philo *, t_timeval time));
void	*check_forks(void *arg);
int		check_starvation(t_philo *philo, t_timeval time);
int		philo_eat(t_philo *philo, t_timeval time);
int		philo_sleep(t_philo *philo, t_timeval time);
int		philo_think(t_philo *philo, t_timeval time);
int		take_forks(t_philo *philo, t_timeval time);
void	put_back_forks(t_philo *philo);
int		refresh_unit_time(t_philo *philo, t_timeval time);
void	philo_printf(long time, t_msg msg, t_philo *philo);

int		parent(t_share *share, t_philo *philos, int ind);
void	init_monitor(t_share *share, t_philo *philos);

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

#endif
