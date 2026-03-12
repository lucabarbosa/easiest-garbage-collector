/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:09:23 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 20:52:17 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(const char *s1, const char *set, t_gc **collector);

char	*ft_strtrim(const char *s1, const char *set, t_gc **collector)
{
	char	*trimmed;
	size_t	initial;
	size_t	end_set;
	size_t	i;

	if (s1 == 0 || set == 0)
		return (0);
	initial = 0;
	while (s1[initial] && ft_strchr((char *)set, s1[initial]))
		initial++;
	end_set = ft_strlen(s1);
	while (end_set > initial && ft_strchr((char *)set, s1[end_set - 1]))
		end_set--;
	trimmed = (char *)gc_malloc(collector, end_set - initial + 1);
	if (trimmed == NULL)
		return (NULL);
	i = 0;
	while (initial < end_set)
		trimmed[i++] = s1[initial++];
	trimmed[i] = '\0';
	return (trimmed);
}
