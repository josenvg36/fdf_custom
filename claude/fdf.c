/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose <jose@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:14:10 by jose              #+#    #+#             */
/*   Updated: 2024/07/07 21:40:22 by jose             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int key_press(int keycode, t_data *data)
{
    if (keycode == 65307) // Escape key
    {
        free_data(data);
        exit(0);
    }
    else if (keycode == 65361) // Left arrow
        data->offset_x -= 10;
    else if (keycode == 65363) // Right arrow
        data->offset_x += 10;
    else if (keycode == 65362) // Up arrow
        data->offset_y -= 10;
    else if (keycode == 65364) // Down arrow
        data->offset_y += 10;

    // Clear the window and redraw the grid
    mlx_clear_window(data->mlx, data->win);
    draw_isometric_grid(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);

    return 0;
}

int mouse_scroll(int button, int x, int y, t_data *data)
{
    (void)x;
    (void)y;
    if (button == 4) // Scroll up
        data->zoom *= ZOOM_FACTOR;
    else if (button == 5) // Scroll down
        data->zoom /= ZOOM_FACTOR;

    // Clear the window and redraw the grid
    mlx_clear_window(data->mlx, data->win);
    draw_isometric_grid(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);

    return 0;
}

void my_mlx_pp(t_data *data, int x, int y, int color)
{
    char *dst;

    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
        *(unsigned int *)dst = color;
    }
}

void draw_line(t_data *data, t_line *line, int color)
{
    int dx;
    int sx;
    int dy;
    int sy;
    int err;
    int e2;

    dx = abs(line->p1.x - line->p0.x);
    if (line->p0.x < line->p1.x)
        sx = 1;
    else
        sx = -1;
    dy = -abs(line->p1.y - line->p0.y);
    if (line->p0.y < line->p1.y)
        sy = 1;
    else
        sy = -1;
    err = dx + dy;
    e2 = 0;
    while (1)
    {
        my_mlx_pp(data, line->p0.x, line->p0.y, color);
        if (line->p0.x == line->p1.x && line->p0.y == line->p1.y)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            line->p0.x += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            line->p0.y += sy;
        }
    }
}


t_point to_isometric(t_data *data, int x, int y, int z)
{
    t_point iso;
    double elevation_scale;

    elevation_scale = data->base_elevation * data->zoom;
    iso.x = (x - y) * cos(M_PI / 6) * data->zoom + WINDOW_WIDTH / 2 + data->offset_x;
    iso.y = (x + y) * sin(M_PI / 6) * data->zoom - z * elevation_scale + WINDOW_HEIGHT / 2 + data->offset_y;
    return iso;
}

void	draw_isometric_grid(t_data *data)
{
    int x;
    int y;
    t_point p1;
    t_point p2;
    t_line line;
    int z;
    int color;

    // Clear the image
    ft_memset(data->addr, 0, WINDOW_WIDTH * WINDOW_HEIGHT * (data->bpp / 8));

    y = 0;
    while (y < data->height)
    {
        x = 0;
        while (x < data->width)
        {
            z = data->elevations[y][x] * data->base_elevation * data->zoom;
            color = data->colors[y][x];
            p1 = to_isometric(data, x * 40, y * 40, z);

            if (x < data->width - 1)
            {
                int z2 = data->elevations[y][x + 1] * data->base_elevation * data->zoom;
                p2 = to_isometric(data, (x + 1) * 40, y * 40, z2);
                line.p0 = p1;
                line.p1 = p2;
                draw_line(data, &line, color);
            }

            if (y < data->height - 1)
            {
                int z2 = data->elevations[y + 1][x] * data->base_elevation * data->zoom;
                p2 = to_isometric(data, x * 40, (y + 1) * 40, z2);
                line.p0 = p1;
                line.p1 = p2;
                draw_line(data, &line, color);
            }

            // Draw vertical line
            p2 = to_isometric(data, x * 40, y * 40, 0);
            // line.p0 = p1;
            // line.p1 = p2;
            // draw_line(data, &line, 0x0000FF00);
            x++;
        }
        y++;
    }
}
void free_2d_array(int **array, int height)
{
	int i;

	i = 0;
	while (i < height)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void free_data(t_data *data)
{
    // int i;

	//free_i(data->img);
    if (data->img)
    {
        mlx_destroy_image(data->mlx, data->img);
        data->img = NULL;
    }

	//free(data->win);
    if (data->win)
    {
        mlx_destroy_window(data->mlx, data->win);
        data->win = NULL;
    }

    if (data->mlx)
    {
        mlx_destroy_display(data->mlx);
        free(data->mlx);
        data->mlx = NULL;
    }

    if (data->elevations)
    {
        // i = 0;
        // while (i < data->height)
        // {
        //     if (data->elevations[i])
        //     {
        //         free(data->elevations[i]);
        //         data->elevations[i] = NULL;
        //     }
        //     i++;
        // }
		free_2d_array(data->elevations, data->height);
        // free(data->elevations);
        data->elevations = NULL;
    }

    if (data->colors)
    {
        // i = 0;
        // while (i < data->height)
        // {
        //     if (data->colors[i])
        //     {
        //         free(data->colors[i]);
        //         data->colors[i] = NULL;
        //     }
        //     i++;
        // }
        // free(data->colors);
		free_2d_array(data->colors, data->height);
        data->colors = NULL;
    }
}

int **allocate_2d_array(int width, int height)
{
    int **array;
    int i;

    array = (int **)malloc(sizeof(int *) * height);
    if (!array)
        return NULL;
    i = 0;
    while (i < height)
    {
        array[i] = (int *)malloc(sizeof(int) * width);
        if (!array[i])
        {
            int j = 0;
            while (j < i)
            {
                free(array[j]);
                j++;
            }
            free(array);
            return NULL;
        }
        i++;
    }
    return array;
}

void parse_first_line(char *line, t_data *data)
{
    char **tokens = ft_split(line, ' ');
    data->width = 0;

    while (tokens[data->width])
        data->width++;

    int i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

int	check_file(const char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void count_lines_and_width(const char *filename, t_data *data)
{
    int fd;
    char *line;
    int line_count;

    fd = check_file(filename);
	line_count = 0;
    while ((line = get_next_line(fd)))
    {
        if (line_count == 0)
        {
            parse_first_line(line, data);
        }
        line_count++;
        free(line);
    }
    data->height = line_count;

    close(fd);
}

void allocate_arrays(t_data *data)
{
    data->elevations = allocate_2d_array(data->width, data->height);
    data->colors = allocate_2d_array(data->width, data->height);

    if (!data->elevations || !data->colors)
    {
        free_data(data);
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
}

void process_token(char *token, int *elevation, int *color)
{
    char *comma = ft_strchr(token, ',');
    if (comma)
    {
        *comma = '\0';
        *elevation = ft_atoi(token);
        *color = strtol(comma + 1, NULL, 16);
    }
    else
    {
        *elevation = ft_atoi(token);
        *color = 0x00FF00; // Default color
    }
}


void fill_elevations_and_colors(const char *filename, t_data *data)
{
    int fd;
    char *line;
    int row;

    // fd = open(filename, O_RDONLY);
    fd = check_file(filename);

    row = 0;
    while ((line = get_next_line(fd)))
    {
        char **tokens = ft_split(line, ' ');
        int col = 0;
        while (col < data->width)
        {
            process_token(tokens[col], &data->elevations[row][col], &data->colors[row][col]);
            col++;
        }
        int i = 0;
        while (tokens[i])
        {
            free(tokens[i]);
            i++;
        }
        free(tokens);
        free(line);
        row++;
    }

    close(fd);
}

void parse_map_file(const char *filename, t_data *data)
{
    count_lines_and_width(filename, data);
    allocate_arrays(data);
    fill_elevations_and_colors(filename, data);
}

// void parse_map_file(const char *filename, t_data *data)
// {
//     int fd;
//     char *line;
//     int line_count;
//     int row;

//     fd = open(filename, O_RDONLY);
//     if (fd < 0)
//     {
//         perror("Error opening file");
//         exit(EXIT_FAILURE);
//     }

//     line_count = 0;
//     data->width = 0;

//     while ((line = get_next_line(fd)))
//     {
//         if (line_count == 0)
//         {
//             char **tokens = ft_split(line, ' ');
//             while (tokens[data->width])
//                 data->width++;
//             int i = 0;
//             while (tokens[i])
//             {
//                 free(tokens[i]);
//                 i++;
//             }
//             free(tokens);
//         }
//         line_count++;
//         free(line);
//     }
//     data->height = line_count;

//     close(fd);

//     data->elevations = allocate_2d_array(data->width, data->height);
//     data->colors = allocate_2d_array(data->width, data->height);

//     if (!data->elevations || !data->colors)
//     {
//         free_data(data);
//         perror("Error allocating memory");
//         exit(EXIT_FAILURE);
//     }

//     fd = open(filename, O_RDONLY);
//     if (fd < 0)
//     {
//         perror("Error opening file");
//         free_data(data);
//         exit(EXIT_FAILURE);
//     }

//     row = 0;
//     while ((line = get_next_line(fd)))
//     {
//         char **tokens = ft_split(line, ' ');
//         int col = 0;
//         while (col < data->width)
//         {
//             char *token = tokens[col];
//             char *comma = ft_strchr(token, ',');
//             if (comma)
//             {
//                 *comma = '\0';
//                 data->elevations[row][col] = ft_atoi(token);
//                 data->colors[row][col] = strtol(comma + 1, NULL, 16);
//             }
//             else
//             {
//                 data->elevations[row][col] = ft_atoi(token);
//                 data->colors[row][col] = 0x00FF00; // Default color
//             }
//             col++;
//         }
//         int i = 0;
//         while (tokens[i])
//         {
//             free(tokens[i]);
//             i++;
//         }
//         free(tokens);
//         free(line);
//         row++;
//     }

//     close(fd);
// }

int init_data(t_data *data)
{
    data->mlx = mlx_init();
    if (data->mlx == NULL)
        return (1);
    data->win = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Isometric Grid");
    if (data->win == NULL)
    {
        mlx_destroy_display(data->mlx);
        free(data->mlx);
        return (1);
    }
    data->img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (data->img == NULL)
    {
        mlx_destroy_window(data->mlx, data->win);
        mlx_destroy_display(data->mlx);
        free(data->mlx);
        return (1);
    }
    data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_length, &data->endian);
    data->offset_x = 0;
    data->offset_y = 0;
    data->zoom = 1.0;
    data->base_elevation = ELEVATION_SCALE;
    return (0);
}

void start_mlx_loop(t_data *data)
{
	mlx_key_hook(data->win, key_press, data);
	mlx_mouse_hook(data->win, mouse_scroll, data);
	draw_isometric_grid(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_loop(data->mlx);
}

void cleanup_data(t_data *data)
{
	if (data->img)
	{
		mlx_destroy_image(data->mlx, data->img);
		data->img = NULL;
	}
    if (data->win)
    {
        mlx_destroy_window(data->mlx, data->win);
        data->win = NULL;
    }
    if (data->mlx)
    {
        mlx_destroy_display(data->mlx);
        free(data->mlx);
        data->mlx = NULL;
    }
	free_data(data);
}


int main(int argc, char **argv)
{
    t_data data;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1;
    }
    if (init_data(&data) != 0)
    {
        fprintf(stderr, "Error initializing data\n");
        return 1;
    }
    parse_map_file(argv[1], &data);
    start_mlx_loop(&data);
    cleanup_data(&data);
    return (0);
}