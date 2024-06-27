/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 16:00:29 by jnajul            #+#    #+#             */
/*   Updated: 2024/06/27 16:48:34 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int key_press(int keycode, t_data *data)
{
    if (keycode == 65307) // Escape key
        exit(0);
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

    return (0);
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

    return (0);
}

void my_mlx_pp(t_data *data, int x, int y, int color)
{
    char    *dst;

    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
        *(unsigned int *)dst = color;
    }
}

void draw_line(t_data *data, int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1)
    {
        my_mlx_pp(data, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

t_point to_isometric(t_data *data, int x, int y, int z)
{
    t_point iso;
    iso.x = (x - y) * cos(M_PI / 6) * data->zoom + WINDOW_WIDTH / 2 + data->offset_x;
    iso.y = (x + y) * sin(M_PI / 6) * data->zoom - z + WINDOW_HEIGHT / 2 + data->offset_y;
    return iso;
}

void draw_isometric_grid(t_data *data)
{
    int x, y;
    t_point p1, p2;

    // Clear the image
    ft_memset(data->addr, 0, WINDOW_WIDTH * WINDOW_HEIGHT * (data->bpp / 8));

    for (y = 0; y < data->height; y++)
    {
        for (x = 0; x < data->width; x++)
        {
            int z = data->elevations[y][x] * ELEVATION_SCALE;
            p1 = to_isometric(data, x * 40, y * 40, z);

            if (x < data->width - 1)
            {
                int z2 = data->elevations[y][x + 1] * ELEVATION_SCALE;
                p2 = to_isometric(data, (x + 1) * 40, y * 40, z2);
                draw_line(data, p1.x, p1.y, p2.x, p2.y, 0x0000FF00);
            }

            if (y < data->height - 1)
            {
                int z2 = data->elevations[y + 1][x] * ELEVATION_SCALE;
                p2 = to_isometric(data, x * 40, (y + 1) * 40, z2);
                draw_line(data, p1.x, p1.y, p2.x, p2.y, 0x0000FF00);
            }

            // Draw vertical line
            p2 = to_isometric(data, x * 40, y * 40, 0);
            draw_line(data, p1.x, p1.y, p2.x, p2.y, 0x0000FF00);
        }
    }
}

void free_map(t_data *data)
{
    for (int i = 0; i < data->height; i++)
        free(data->elevations[i]);
    free(data->elevations);
}

int **allocate_2d_array(int width, int height)
{
    int **array = (int **)malloc(sizeof(int *) * height);
    for (int i = 0; i < height; i++)
        array[i] = (int *)malloc(sizeof(int) * width);
    return array;
}

void parse_map_file(const char *filename, t_data *data)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line;
    int line_count = 0;
    data->width = 0;

    while ((line = get_next_line(fd)))
    {
        if (line_count == 0)
        {
            char **tokens = ft_split(line, ' ');
            while (tokens[data->width])
                data->width++;
            for (int i = 0; tokens[i]; i++)
                free(tokens[i]);
            free(tokens);
        }
        line_count++;
        free(line);
    }
    data->height = line_count;

    close(fd);

    data->elevations = allocate_2d_array(data->width, data->height);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int row = 0;
    while ((line = get_next_line(fd)))
    {
        char **tokens = ft_split(line, ' ');
        for (int col = 0; col < data->width; col++)
        {
            data->elevations[row][col] = atoi(tokens[col]);
        }
        for (int i = 0; tokens[i]; i++)
            free(tokens[i]);
        free(tokens);
        free(line);
        row++;
    }

    close(fd);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1;
    }

    t_data data;

    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Isometric Grid");
    if (data.win == NULL)
    {
        return (1);
    }
    data.img = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    data.addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_length, &data.endian);
    data.offset_x = 0;
    data.offset_y = 0;
    data.zoom = 1.0;

    parse_map_file(argv[1], &data);

    mlx_key_hook(data.win, key_press, &data);
    mlx_mouse_hook(data.win, mouse_scroll, &data);
    draw_isometric_grid(&data);
    mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
    mlx_loop(data.mlx);

    free_map(&data);

    return (0);
}
