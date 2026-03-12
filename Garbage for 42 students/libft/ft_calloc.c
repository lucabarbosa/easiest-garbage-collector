/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 20:22:14 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 21:00:29 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size, t_gc **collector);

void	*ft_calloc(size_t nmemb, size_t size, t_gc **collector)
{
	size_t			total;
	size_t			i;
	unsigned char	*ptr;

	if (nmemb == 0 || size == 0)
	{
		ptr = gc_malloc(collector, 1);
		if (ptr == NULL)
			return (NULL);
		return (ptr);
	}
	total = nmemb * size;
	if (total / size != nmemb)
		return (NULL);
	ptr = gc_malloc(collector, total);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < total)
	{
		ptr[i] = '\0';
		i++;
	}
	return (ptr);
}
