/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 16:17:21 by jnajul            #+#    #+#             */
/*   Updated: 2024/07/06 16:26:32 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "fdf.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// Define BUFFER_SIZE if not already defined
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 32
#endif

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	i;

	i = ft_strlen(s);
	ptr = (char *)malloc(sizeof(char) * (i + 1));
	if (!ptr)
		return (NULL);
	for (size_t j = 0; j <= i; j++)
		ptr[j] = s[j];
	return (ptr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	str_len;
	size_t	i;

	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	ptr = (char *)malloc(len + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	ptr = (char *)malloc((sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1)));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = '\0';
	return (ptr);
}

char	*set_line(char *line)
{
	int		i;
	char	*line_remain;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (NULL);
	line_remain = ft_substr(line, i + 1, ft_strlen(line) - i - 1);
	line[i + 1] = '\0';
	return (line_remain);
}

char	*fill_buffer(int fd, char *last_char)
{
	int		buffer_len;
	char	buffer[BUFFER_SIZE + 1];
	char	*tmp;

	buffer_len = 1;
	while (buffer_len)
	{
		buffer_len = read(fd, buffer, BUFFER_SIZE);
		if (buffer_len <= 0)
			break ;
		buffer[buffer_len] = '\0';
		if (!last_char)
			last_char = ft_strdup("");
		tmp = last_char;
		last_char = ft_strjoin(tmp, buffer);
		free(tmp);
		if (ft_strchr(last_char, '\n'))
			break ;
	}
	return (last_char);
}

char	*get_next_line(int fd)
{
	static char	*last_char;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	last_char = fill_buffer(fd, last_char);
	if (!last_char)
		return (NULL);
	line = ft_strdup(last_char);
	free(last_char);
	last_char = set_line(line);
	return (line);
}
