NAME = fdf
CC = gcc
CFLAGS = -Wall -Wextra -Werror
MLX_DIR = ../minilibx-linux
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm
SRCS = fdf.c \
    	../get_next_line/get_next_line.c 
OBJS = $(SRCS:.c=.o)
LIBFT = ../libft/libft.a
INCLUDES = -I get_next_line -I libft -I $(MLX_DIR) -I .

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX_DIR)/libmlx.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_FLAGS) -o $(NAME)

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