#include "garbage_colector.h"
#include <stdio.h>
#include <string.h>

int	main(int argc, char **argv)
{
	t_gc	*shell;
	char	*text;

	if (argc == 1)
		return (0);
	shell = NULL;
	text = gc_malloc(&shell, sizeof(char) * strlen(argv[1]));
	text = argv[1];
	printf("\n%s\n", text);
	gc_clear(&shell);
	return (0);
}