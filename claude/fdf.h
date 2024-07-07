/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose <jose@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 16:03:22 by jnajul            #+#    #+#             */
/*   Updated: 2024/07/07 17:29:28 by jose             ###   ########.fr       */
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
# include <stddef.h>
# include <fcntl.h>
# include <unistd.h>

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080
# define ELEVATION_SCALE 2.5
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
    int     **colors;
    int     width;
    int     height;
    double  base_elevation;
}   t_data;


typedef struct s_point
{
    int x;
    int y;
} t_point;

typedef struct s_line
{
    t_point p0;
    t_point p1;
}   t_line;

// Function prototypes
void    draw_isometric_grid(t_data *data);
void    my_mlx_pp(t_data *data, int x, int y, int color);
void    draw_line(t_data *data, t_line *line, int color);
void    parse_map_file(const char *filename, t_data *data);
void    free_data(t_data *data);
// char	*get_next_line(int fd);

#endif
