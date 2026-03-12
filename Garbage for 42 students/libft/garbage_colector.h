/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_colector.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 00:25:41 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 16:13:13 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLECTOR_H
# define GARBAGE_COLECTOR_H

# include <stdlib.h>

typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
}	t_gc;

void	gc_clear(t_gc **gc);
void	gc_add(t_gc **gc, void *ptr);
void	gc_free(t_gc **gc, void *ptr);
void	*gc_malloc(t_gc **gc, size_t size);

#endif
