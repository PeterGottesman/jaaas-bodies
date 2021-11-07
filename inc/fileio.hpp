#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>

/**
 * load_file
 *
 * Returns a pointer to a newly allocated string containing entirety
 * of loaded file. This buffer should be freed by the caller.
 * 
 */
static char *load_file(std::string filename, int *length)
{
	FILE *f;
	char *output;

	f = fopen(filename.c_str(), "r");
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
