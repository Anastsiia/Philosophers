NAME = philo

CC = gcc

FLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c

HEADERS = philosophers.h

OBJS = $(SRCS:c=o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
