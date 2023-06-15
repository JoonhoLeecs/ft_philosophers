NAME = philo_bonus

CC = cc
CFLAGS = -Werror -Wall -Wextra
LDFLAGS = -pthread

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

HEADER = \
philo_bonus.h

BONUS_SRCS = \
check_init_args_bonus.c \
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
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(BONUS_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean :
	$(RM) $(RMFLAGS) $(BONUS_OBJS)

fclean :
	$(MAKE)	clean
	$(RM) $(RMFLAGS) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY : all bonus clean fclean re
