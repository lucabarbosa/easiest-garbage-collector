/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:34:24 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 20:52:20 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd, t_gc **collector);
char	*read_line(int fd, char *remaining, char *buffer, t_gc **collector);
char	*save_file(char *line, t_gc **collector);

char	*get_next_line(int fd, t_gc **collector)
{
	static char	*remaining[MAX_FD];
	char		*line;
	char		*buffer;

	buffer = gc_malloc(collector, (BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		gc_free(collector, buffer);
		remaining[fd] = NULL;
		buffer = NULL;
		return (NULL);
	}
	line = read_line(fd, remaining[fd], buffer, collector);
	gc_free(collector, buffer);
	buffer = NULL;
	if (!line)
	{
		remaining[fd] = NULL;
		return (NULL);
	}
	remaining[fd] = save_file(line, collector);
	return (line);
}

char	*read_line(int fd, char *remaining, char *buffer, t_gc **collector)
{
	int		bytes_read;
	char	*temp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			return (NULL);
		}
		else if (bytes_read == 0)
			return (remaining);
		buffer[bytes_read] = '\0';
		if (!remaining)
			remaining = ft_strdup("", collector);
		temp = remaining;
		remaining = ft_strjoin(temp, buffer, collector);
		gc_free(collector, temp);
		temp = NULL;
		if (ft_strchr(buffer, '\n'))
			return (remaining);
	}
	return (remaining);
}

char	*save_file(char *line, t_gc **collector)
{
	char	*rest;
	size_t	i;
	size_t	size_line;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
	{
		return (NULL);
	}
	size_line = (ft_strlen(line) - i);
	rest = ft_substr(line, i + 1, size_line, collector);
	if (*rest == '\0')
	{
		gc_free(collector, rest);
		rest = NULL;
	}
	line[i + 1] = '\0';
	return (rest);
}
