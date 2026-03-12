/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:51:22 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 20:52:19 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# ifndef MAX_FD
#  define MAX_FD 1024
# endif

# include <unistd.h>
# include "libft.h"
# include "garbage_colector.h"

char	*get_next_line(int fd, t_gc **collector);
char	*read_line(int fd, char *remaining, char *buffer, t_gc **collector);
char	*save_file(char *line, t_gc **collector);

#endif
