/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:04:37 by jnajul            #+#    #+#             */
/*   Updated: 2024/06/27 17:10:40 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	key_press(int keycode, t_data *data)
{
	if (keycode == 65307)
		exit (0);
	mlx_clear_window(data->mlx, data->win);
	return (0);
}

int	main(void)
{
	t_data	data;

	data.mlx = mlx_init();
	data.win = mlx_get_data_addr(data.img, &data.bpp, &data.line_length, \
		&data.endian);
	mlx_key_hook(data.win, key_press, &data);
	return (0);
}