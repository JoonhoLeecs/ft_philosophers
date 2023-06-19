NAME = philo

CC = cc
# CFLAGS = -Werror -Wall -Wextra -fsanitize=thread -g
CFLAGS = -Werror -Wall -Wextra
LDFLAGS = -pthread

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

HEADER = \
philosophers.h

MANDATORY_SRCS = \
check_init_args.c \
init_philos.c \
philo_utils.c \
philo_utils2.c \
philosophers.c \
philo_routine.c \
philo_routine2.c \
philo_routine3.c \
monitoring.c \
philo_print.c

MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(MANDATORY_OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(MANDATORY_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean :
	$(RM) $(RMFLAGS) $(MANDATORY_OBJS)

fclean :
	$(MAKE)	clean
	$(RM) $(RMFLAGS) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY : all clean fclean re
