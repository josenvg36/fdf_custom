/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 16:03:22 by jnajul            #+#    #+#             */
/*   Updated: 2024/06/27 17:13:21 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../minilibx-linux/mlx.h"
# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include <string.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080
# define ELEVATION_SCALE 8
# define ZOOM_FACTOR 1.1

typedef struct s_data
{
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bpp;
    int     line_length;
    int     endian;
    int     offset_x;
    int     offset_y;
    double  zoom;
    int     **elevations;
    int     width;
    int     height;
}   t_data;

typedef struct s_point
{
    int x;
    int y;
} t_point;

// Function prototypes
void    draw_isometric_grid(t_data *data);
void    my_mlx_pp(t_data *data, int x, int y, int color);
void    draw_line(t_data *data, int x0, int y0, int x1, int y1, int color);
void    parse_map_file(const char *filename, t_data *data);
void    free_map(t_data *data);

#endif
