/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:29:02 by jnajul            #+#    #+#             */
/*   Updated: 2024/06/25 15:56:00 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <math.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GRID_SIZE 20
#define CELL_SIZE 40
#define ELEVATION_SCALE 8

typedef struct s_point {
    int x;
    int y;
} t_point;

void *mlx;
void *win;
int landscape[GRID_SIZE][GRID_SIZE];

void draw_line(int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        mlx_pixel_put(mlx, win, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

t_point to_isometric(int x, int y, int z) {
    t_point iso;
    iso.x = (x - y) * cos(M_PI / 6) + WINDOW_WIDTH / 2;
    iso.y = (x + y) * sin(M_PI / 6) - z + WINDOW_HEIGHT / 2;
    return iso;
}

void generate_landscape() {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            landscape[y][x] = rand() % 10;
        }
    }
}

void draw_landscape() {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            int height = landscape[y][x] * ELEVATION_SCALE;
            t_point pos = to_isometric(x * CELL_SIZE, y * CELL_SIZE, height);
            
            draw_line(pos.x, pos.y, pos.x, pos.y + height, 0x00FF00);
            
            if (x < GRID_SIZE - 1) {
                t_point next = to_isometric((x+1) * CELL_SIZE, y * CELL_SIZE, landscape[y][x+1] * ELEVATION_SCALE);
                draw_line(pos.x, pos.y, next.x, next.y, 0x00FF00);
            }
            if (y < GRID_SIZE - 1) {
                t_point next = to_isometric(x * CELL_SIZE, (y+1) * CELL_SIZE, landscape[y+1][x] * ELEVATION_SCALE);
                draw_line(pos.x, pos.y, next.x, next.y, 0x00FF00);
            }
        }
    }
}

int main() {
    mlx = mlx_init();
    win = mlx_new_window(mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cyberpunk Isometric Projection");

    generate_landscape();
    draw_landscape();

    mlx_loop(mlx);

    return 0;
}