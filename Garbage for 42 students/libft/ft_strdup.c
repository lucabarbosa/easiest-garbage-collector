/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 20:31:27 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 20:51:58 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s, t_gc **collector);

char	*ft_strdup(const char *s, t_gc **collector)
{
	size_t	size_s;
	char	*dest;

	size_s = ft_strlen(s) + 1;
	dest = (char *)gc_malloc(collector, size_s * sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_memcpy(dest, (const void *)s, size_s);
	return (dest);
}
