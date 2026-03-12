/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:51:22 by lbento            #+#    #+#             */
/*   Updated: 2026/02/10 12:57:14 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# include "garbage_colector.h"

char	*get_next_line(int fd, t_gc **collector);
char	*read_line(int fd, char *remaining, char *buffer, t_gc **collector);
char	*save_file(char *line, t_gc **collector);

#endif