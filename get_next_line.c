/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcierra <alcierra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 12:18:55 by alcierra          #+#    #+#             */
/*   Updated: 2021/10/20 17:14:24 by alcierra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"



#include <stdio.h>

/*
char	*ft_get_str_fd(int fd, size_t i)
{
	static char	*str;
	static int	res;
	char		c;

	res = read(fd, &c, 1);
	printf("%c\n", c);
	if (res == -1)
	{
		str = (char *) malloc(i + 1);
		str[i] = 0;
	}
	else if (c == '\n')
	{
		str = (char *) malloc(i + 2);
		str[i] = c;
		str[i + 1] = 0;
	}
	else
	{
		(void) ft_get_str_fd(fd, i + 1);
		str[i] = c;
	}
	return (str);
}

char	*get_next_line(int fd)
{
	char	*str;

	str = ft_get_str_fd(fd, 0);
	return (str);
}
*/

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*temp_dst;
	const char	*temp_src;

	temp_dst = (char *) dst;
	temp_src = (char *) src;
	if (dst == (void *) 0 && src == (void *) 0)
		return (dst);
	if (dst < src)
	{
		while (len--)
			*temp_dst++ = *temp_src++;
	}
	else
	{
		temp_dst += len - 1;
		temp_src += len - 1;
		while (len--)
			*temp_dst-- = *temp_src--;
	}
	return (dst);
}

char	*ft_proccess(char *buff, char *line, size_t bytes_read, char **ptr_n)
{
	char	*new_line;
	char	changed_symb;

	ptr_n[0] = ft_strchr((const char *) buff, (int) '\n');
	if (ptr_n[0])
	{
		changed_symb = ptr_n[0][1];
		ptr_n[0][1] = 0;
	}
	new_line = ft_strjoin(line, buff);
	buff[0] = 0;
	if (line[0])
		free(line);
	if (ptr_n[0])
	{
		ptr_n[0][1] = changed_symb;
		ft_memmove(buff, ptr_n[0] + 1, bytes_read - (ptr_n[0] - buff));
		buff[bytes_read - (ptr_n[0] - buff + 1)] = 0;
	}
	return (new_line);
}

char	*get_next_line(int fd)
{
	static char	buff_read[BUFFER_SIZE + 1];
	size_t		bytes_read;
	char		*line;
	char		*ptr_n;
	size_t		buff_len;

	buff_len = ft_strlen(buff_read);
	if (buff_len > (size_t) BUFFER_SIZE)
		buff_len = 0;
	line = "";
	ptr_n = NULL;
	if (buff_len)
		line = ft_proccess(buff_read, line, buff_len + 1, &ptr_n);
	buff_len = ft_strlen(buff_read);
	if (ptr_n)
		return (line);
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	bytes_read = read(fd, buff_read + buff_len, BUFFER_SIZE - buff_len);
	if (bytes_read <= (size_t) 0)
		return (0);
	while (!ptr_n && bytes_read)
	{
		buff_read[bytes_read + buff_len] = 0;
		buff_len = 0;
		line = ft_proccess(buff_read, line, bytes_read + buff_len, &ptr_n);
		if (!ptr_n)
			bytes_read = read(fd, buff_read + buff_len, BUFFER_SIZE - buff_len);
	}
	return (line);
}
