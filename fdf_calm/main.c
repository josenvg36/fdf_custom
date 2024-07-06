/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:44:02 by jnajul            #+#    #+#             */
/*   Updated: 2024/07/06 20:35:12 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	key_hook(int keycode, t_data *data)
{
	if (keycode == XK_Escape)
	{
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit (1);
	}
	return (0);
}

void	draw_grid(t_data *data)
{
	int	x;
	int	y;

	x = 0;
	while (x <= (data->map.width * CELL_SIZE))
	{
		y = 0;
		while (y <= (data->map.height * CELL_SIZE))
		{
			mlx_pixel_put(data->mlx, data->win, x, y, 0xFFFFFF);
			y++;
		}
		x += CELL_SIZE;
	}
	y = 0;
	while (y <= (data->map.height * CELL_SIZE))
	{
		x = 0;
		while (x <= data->map.width * CELL_SIZE)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, 0xFFFFFF);
			x++;
		}
		y += CELL_SIZE;
	}
}

// int	**allocate_2d_array(t_map	map)
// {
// 	int	**array;
// 	int	i;

// 	i = 0;
// 	array = (int **)malloc(sizeof(int *) * map.height);
// 	while (i < map.height)
// 	{
// 		array[i] = (int *)malloc(sizeof(int) * map.width);
// 		i++;
// 	}
// 	return (array);
// }

// void	parse_elevation(t_data *data, char *line, int row)
// {
// 	char	**tokens;
// 	int		col;

// 	tokens = ft_split(line, ' ');
// 	col = 0;
// 	while (col < data->map.width)
// 	{
// 		data->map.elevations[row][col] = atoi(tokens[col]);
// 		col++;
// 		free(tokens[col - 1]);
// 	}
// 	free(line);
// }

// void	read_elevations(const char *file, t_data *data)
// {
// 	int		fd;
// 	char	*line;
// 	int		row;

// 	fd = open(file, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("Error opening file");
// 		exit(1);
// 	}
// 	row = 0;
// 	line = get_next_line(fd);
// 	while (line && row < data->map.height)
// 	{
// 		parse_elevation(data, line, row);
// 		row++;
// 		line = get_next_line(fd);
// 	}
// 	free(line);
// 	close(fd);
// }

void	read_map(char *filename, t_data *data)
{
	int		fd;
	// t_map	*map;
	char	*line;
	char	**tokens;

	// map = (t_map *)calloc(1, sizeof(t_map));
	// if (!map)
	// 	return ;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("Error reading file");
		exit (1);
	}
	data->map.height = 0;
	data->map.width = 0;
	line = get_next_line(fd);
	tokens = ft_split(line, ' ');
	// READ THE WIDTH
	while (tokens[data->map.width])
	{
		data->map.width++;
		free(tokens[data->map.width - 1]);
	}
	printf("Map width: %i\n", data->map.width);
	free(tokens);
	// READ THE HEIGHT
	while (line)
	{
		data->map.height++;
		free(line);
		line = get_next_line(fd);
	}
	data->map.elevations = allocate_2d_array(data->map);
	printf("Map height: %i\n", data->map.height);
	close(fd);
}


int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
	{
		printf("Error %s program requires 2 arguments", argv[0]);
		return (1);
	}
	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);
	data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "FDF");
	if (!data.win)
	{
		mlx_destroy_display(data.mlx);
		free(data.mlx);
		return (1);
	}
	data.img = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	data.addr = mlx_get_data_addr(data.img, &data.bpp, \
		&data.line_len, &data.endian);
	read_map(argv[1], &data);
	mlx_key_hook(data.win, key_hook, &data);
	draw_grid(&data);
	mlx_loop(data.mlx);
	mlx_destroy_window(data.mlx, data.win);
	mlx_destroy_display(data.mlx);
	free(data.mlx);
	return (0);
}
