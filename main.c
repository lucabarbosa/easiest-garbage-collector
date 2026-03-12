#include "garbage_colector.h"
#include <stdio.h>
#include <string.h>

int	main()
{
	t_gc	*collector;
	int		n;
	int		*vector;

	collector = NULL;				//		<------ Always initialize the collector
	printf("How many numbers do you want to alloc ? ");
	scanf("%d", &n);
	vector = (int *) gc_malloc(&collector ,n * sizeof(int)); //		Use like a normal malloc, passing your size and the collector
	if (!vector)
		return (printf("Error during allocation.\n"), 1);
	for (int i = 0; i < n; i++)
	{
		printf("Insert number %d: ", i + 1);
		scanf("%d", &vector[i]);
	}
	printf("Numbers allocated: ");
	for (int i = 0; i < n; i++)
		printf("%d ", vector[i]);
	gc_free(&collector, vector);			//		<---- You can use like a formal free too, freeing allocated memory. Always putting the collector that stores the pointer.
	gc_clear(&collector);				//		<---- Or you can use the gc_clear to free all the memory allocated during your program, cleaning including the garbage.
	return (0);
}
