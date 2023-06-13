NAME = philo

CC = cc
CFLAGS = -Werror -Wall -Wextra
# CFLAGS = -Werror -Wall -Wextra -fsanitize=thread -g
LDFLAGS = -pthread

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

HEADER = \
philosophers.h

# INCLUDES = -I./mandatory

MANDATORY_SRCS = \
check_init_args.c \
philo_utils.c \
philosophers.c \
philo_routine.c \
monitoring.c


MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(MANDATORY_OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(MANDATORY_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ $(INCLUDES)

clean :
	$(RM) $(RMFLAGS) $(MANDATORY_OBJS)

fclean :
	$(MAKE)	clean
	$(RM) $(RMFLAGS) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY : all clean fclean re
