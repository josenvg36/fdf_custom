/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:44:35 by jnajul            #+#    #+#             */
/*   Updated: 2024/07/06 19:23:19 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../minilibx-linux/mlx.h"
# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include <X11/keysym.h>
# include <stdio.h>
# include <fcntl.h>

# define WINDOW_HEIGHT 1000
# define WINDOW_WIDTH 1000
# define CELL_SIZE 45

typedef struct s_map
{
	int	height;
	int	width;
	int	**elevations;
}	t_map;


typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		endian;
	int		line_len;
	t_map	map;
}	t_data;

char	*get_next_line(int fd);

#endif