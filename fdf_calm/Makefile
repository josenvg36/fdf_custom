NAME = fdf

CC = gcc
MLX_DIR = ../minilibx-linux

CFLAGS = -Wall -Wextra -Werror
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lX11 -lXext -L../libft -lft

SRCS = main.c get_next_line.c
OBJS = $(SRCS:.c=.o)
LIBFT = ../libft/libft.a
INCLUDES = -I get_next_line -I libft -I $(MLX_DIR) -I . 

#Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX_DIR)/libmlx.a
	$(CC) $(CFLAGS) $(LIBFT) $(OBJS) $(MLX_FLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C ../libft

$(MLX_DIR)/libmlx.a:
	$(MAKE) -C $(MLX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C ../libft clean
	$(MAKE) -C $(MLX_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C ../libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re



