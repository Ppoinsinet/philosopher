SRC = threads.c utils.c main.c print.c utils_2.c

INC = philosopher.h

NAME = philo

OBJS = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror -g -fsanitize=thread

CC = clang

LIBS = -lpthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o $(NAME)


.c.o:
	$(CC) $<  $(FLAGS) -c

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)
