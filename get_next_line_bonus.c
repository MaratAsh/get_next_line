/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcierra <alcierra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 12:18:55 by alcierra          #+#    #+#             */
/*   Updated: 2021/11/17 16:29:38 by alcierra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <stdio.h>

#define BUFF_CNT 1024

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
	if (!new_line)
		return (NULL);
	if (ptr_n[0])
	{
		ptr_n[0][1] = changed_symb;
		ft_memmove(buff, ptr_n[0] + 1, bytes_read - (ptr_n[0] - buff));
		buff[bytes_read - (ptr_n[0] - buff + 1)] = 0;
	}
	return (new_line);
}

char	*ft_buff_preproc(char *buff, char *line, char **ptr_n, size_t *buff_len)
{
	*ptr_n = NULL;
	*buff_len = ft_strlen(buff);
	if (*buff_len)
		line = ft_proccess(buff, line, *buff_len + 1, ptr_n);
	*buff_len = ft_strlen(buff);
	return (line);
}

char	*ft_get_current_buffer(int fd)
{
	static char	buff[BUFF_CNT][BUFFER_SIZE + 1];

	if (fd < 0)
		return (NULL);
	return (buff[fd % BUFF_CNT]);
}

char	*get_next_line(int fd)
{
	char		*buff_read;
	size_t		lens[2];
	char		*line;
	char		*ptr_n;

	buff_read = ft_get_current_buffer(fd);
	if (buff_read == NULL)
		return (NULL);
	line = ft_buff_preproc(buff_read, "", &ptr_n, &(lens[1]));
	if (ptr_n)
		return (line);
	lens[0] = read(fd, buff_read + lens[1], BUFFER_SIZE - lens[1]);
	if ((int) lens[0] <= 0 && !(line[0]))
		return (0);
	while (!ptr_n && (int) lens[0] > 0)
	{
		buff_read[lens[0] + lens[1]] = 0;
		lens[1] = 0;
		line = ft_proccess(buff_read, line, lens[0] + lens[1], &ptr_n);
		if (!ptr_n)
			lens[0] = read(fd, buff_read + lens[1], BUFFER_SIZE - lens[1]);
	}
	if (!*line)
		return (NULL);
	return (line);
}
