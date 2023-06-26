NAME = philo_bonus

CC = cc
CFLAGS = -Werror -Wall -Wextra

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

HEADER = \
philo_bonus.h

BONUS_SRCS = \
check_init_args_bonus.c \
init_philos_bonus.c \
philo_utils_bonus.c \
philo_utils2_bonus.c \
philo_bonus.c \
philo_routine_bonus.c \
philo_routine2_bonus.c \
philo_routine3_bonus.c \
parent_bonus.c \
philo_print_bonus.c

BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all : $(NAME)

bonus : $(NAME)

$(NAME) : $(BONUS_OBJS) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(BONUS_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(RMFLAGS) $(BONUS_OBJS)

fclean :
	$(MAKE)	clean
	$(RM) $(RMFLAGS) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY : all bonus clean fclean re
