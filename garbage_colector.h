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
