/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbento <lbento@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 16:58:31 by lbento            #+#    #+#             */
/*   Updated: 2026/03/12 17:45:16 by lbento           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <string.h>

int	main()
{
	t_gc	*collector;
	int		n;
	int		i;
	int		*vector;

	collector = NULL;				//		<------ Always initialize the collector
	ft_putstr_fd("How many numbers do you want to alloc ?\n-> ", STDOUT_FILENO);
	scanf("%i", &n);
	vector = (int *) gc_malloc(&collector ,n * sizeof(int)); //		Use like a normal malloc, passing your size and the collector
	if (!vector)
		return (ft_putendl_fd("Error during allocation.", STDERR_FILENO), 1);
	if (n < 1)
		return (ft_putendl_fd("Input invalid", 2), gc_clear(&collector), 1);
	i = 0;
	while (i < n)
	{
		ft_putstr_fd("Insert number ", STDOUT_FILENO);
		ft_putnbr_fd(i + 1, STDOUT_FILENO);
		ft_putstr_fd(": ", STDOUT_FILENO);
		scanf("%i", &vector[i]);
		if (vector[i] < 1)
			return (ft_putendl_fd("Input invalid", 2), gc_clear(&collector), 1);
		i++;
	}
	ft_putstr_fd("\nNumbers allocated: ", STDOUT_FILENO);
	i = 0;
	while (i < n)
	{
		ft_putnbr_fd(vector[i], STDOUT_FILENO);
		if (i == n - 1)
			ft_putstr_fd("\n", STDOUT_FILENO);
		else
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	gc_free(&collector, vector);			//		<---- You can use like a formal free too, freeing allocated memory. Always putting the collector that stores the pointer.
	gc_clear(&collector);				//		<---- Or you can use the gc_clear to free all the memory allocated during your program, cleaning including the garbage.
	return (0);
}
