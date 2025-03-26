MAKEFLAGS += --no-print-directory

NAME = pipex

CC = gcc
FLAGS = -Wextra -Werror -Wall -g

LIBFT = libft/libft.a

OBJDIR := obj

SRCS = pipex.c processes.c path_parsing.c \

OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "🔧 Compiling files... 🔨\n🛠️  Done. 🛠️"
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $@

$(LIBFT):
	@make -C libft

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
	@make clean -C ./libft

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C ./libft
	@echo "🧹 Cleaning everything... 🧹"

re: fclean all

.PHONY: all clean fclean re