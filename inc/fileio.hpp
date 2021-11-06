#include <stdio.h>
#include <stdlib.h>

/**
 * load_file
 *
 * Returns a pointer to a newly allocated string containing entirety
 * of loaded file. This buffer should be freed by the caller.
 * 
 */
static char *load_file(char *filename, int *length)
{
	FILE *f;
	char *output;

	f = fopen(filename, "r");
	if (f == NULL)
	{
		perror("load_file");
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	*length = ftell(f);

	output = (char *)malloc(*length);

	rewind(f);

	fread(output, 1, *length, f);
	fclose(f);

	return output;
}
