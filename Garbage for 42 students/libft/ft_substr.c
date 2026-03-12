/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:11:16 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 20:52:21 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char *s, unsigned int start, size_t len, t_gc **collector);

static int	ft_substr_len(unsigned int start, size_t len, size_t s_len)
{
	size_t	total;

	total = len;
	if (start + len > s_len)
		total = s_len - start;
	return (total);
}

char	*ft_substr(char *s, unsigned int start, size_t len, t_gc **collector)
{
	char	*substr;
	size_t	i;
	size_t	len_s;
	size_t	substr_len;

	len_s = ft_strlen(s);
	if (start >= len_s || len == 0)
	{
		substr = gc_malloc(collector, 1);
		substr[0] = '\0';
		return (substr);
	}
	substr_len = ft_substr_len(start, len, len_s);
	substr = (char *)gc_malloc(collector, substr_len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < substr_len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
