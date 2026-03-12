/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 19:42:44 by lbento            #+#    #+#             */
/*   Updated: 2026/01/13 21:01:04 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(const char *str, char c);

static void	split_loop(char c, char **splited, const char *s, t_gc **collector);

static char	*fill_word(const char *str, int start, int end, t_gc **collector);

static void	*ft_free(t_gc **collector);

char	**ft_split(char const *s, char c, t_gc **collector)
{
	size_t	total_word;
	char	**splited;

	total_word = word_count(s, c);
	splited = ft_calloc((total_word + 1), sizeof(char *), collector);
	if (!splited)
		return (NULL);
	split_loop(c, splited, s, collector);
	return (splited);
}

static void	split_loop(char c, char **splited, const char *s, t_gc **collector)
{
	int		first;
	size_t	i;
	size_t	j;
	size_t	total_word;

	i = 0;
	j = 0;
	first = -1;
	total_word = word_count(s, c);
	while (j < total_word)
	{
		if (s[i] != c && first < 0)
			first = i;
		else if ((s[i] == c || s[i] == '\0') && first >= 0)
		{
			splited[j] = fill_word(s, first, i, collector);
			if (!splited[j])
				ft_free(collector);
			first = -1;
			j++;
		}
		i++;
	}
}

static int	word_count(const char *str, char c)
{
	int	i;
	int	switc;
	int	word;

	i = 0;
	word = 0;
	switc = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c && switc == 0)
		{
			switc = 1;
			word++;
		}
		if (str[i] == c)
			switc = 0;
		i++;
	}
	return (word);
}

static char	*fill_word(const char *str, int start, int end, t_gc **collector)
{
	char	*word;
	int		i;

	i = 0;
	word = gc_malloc(collector, (end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i] = str[start];
		start++;
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	*ft_free(t_gc **collector)
{
	gc_clear(collector);
	return (NULL);
}
