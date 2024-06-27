/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnajul <jnajul@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:57:29 by jnajul            #+#    #+#             */
/*   Updated: 2024/03/19 19:15:39 by jnajul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

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
	size_t	i;

	i = 0;
	if (c == '\0')
	{
		i = ft_strlen(s);
		return ((char *)&s[i]);
	}
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			return ((char *)&s[i]);
		i++;
	}
	return (0);
}

char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	i;

	i = ft_strlen(s);
	ptr = (char *)malloc(sizeof(char) * (i + 1));
	if (!ptr)
		return (NULL);
	while (*s && i > 0)
	{
		*ptr = *s;
		ptr++;
		s++;
		i--;
	}
	*ptr = '\0';
	return ((char *)ptr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	str_len;
	size_t	i;

	str_len = ft_strlen(s);
	if (!s)
		return (NULL);
	if (start > str_len || len == 0)
		return (ft_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	ptr = (char *) malloc(len + 1);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
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
	int		i;
	int		j;

	ptr = (char *)malloc((sizeof(char) * (ft_strlen(s1) + ft_strlen(s2))) + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = '\0';
	return ((char *)ptr);
}
