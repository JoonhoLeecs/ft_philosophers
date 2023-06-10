NAME = philo_bonus

CC = cc
CFLAGS = -Werror -Wall -Wextra
# CFLAGS = -Werror -Wall -Wextra -g -O3 -fsanitize=address
LDFLAGS = -pthread

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

HEADER = \
philo_bonus.h

# INCLUDES = -I./mandatory

BONUS_SRCS = \
check_init_args_bonus.c \
philo_utils_bonus.c \
philo_bonus.c \
philo_routine_bonus.c \
parent_bonus.c


BONUS_OBJS = $(BONUS_SRCS:.c=.o)

bonus : $(NAME)

$(NAME) : $(BONUS_OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(BONUS_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ $(INCLUDES)

clean :
	$(RM) $(RMFLAGS) $(BONUS_OBJS)

fclean :
	$(MAKE)	clean
	$(RM) $(RMFLAGS) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY : all clean fclean re
