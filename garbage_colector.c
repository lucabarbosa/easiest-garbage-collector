#include "garbage_colector.h"

void	gc_clear(t_gc **gc);
int	gc_add(t_gc **gc, void *ptr);
void	gc_free(t_gc **gc, void *ptr);
void	*gc_malloc(t_gc **gc, size_t size);

void	*gc_malloc(t_gc **gc, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (gc_add(gc, ptr))
		return (NULL);
	return (ptr);
}

int	gc_add(t_gc **gc, void *ptr)
{
	t_gc	*new_node;

	if (!ptr)
		return (1);
	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		free(ptr);
		return (1);
	}
	new_node->ptr = ptr;
	new_node->next = *gc;
	*gc = new_node;
	return (0);
}

void	gc_clear(t_gc **gc)
{
	t_gc	*current;
	t_gc	*next;

	if (!gc || !*gc)
		return ;
	current = *gc;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	*gc = NULL;
}

void	gc_free(t_gc **gc, void *ptr)
{
	t_gc	*current;
	t_gc	*prev;

	if (!gc || !*gc || !ptr)
		return ;
	current = *gc;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				*gc = current->next;
			free(current->ptr);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
