#include "./input.h"

#include "./utilities.h"

#include <limits.h>


int readUntil(FILE *f, char *output, int maxLength, char delimiter)
{
	int c = 0;
	int bytesRead = 0;
	int bytesWritten = 0;
	while (1) {
		if (bytesRead == INT_MAX) panic("read too far before delimiter reached\n");
		c = getc(f);
		bytesRead++;

		if (c == delimiter) break;
		else if (c == EOF) {
			bytesRead *= -1;
			break;
		}

		if (bytesRead <= maxLength) {
			output[bytesRead - 1] = c;
			bytesWritten++;
		}
	}

	if (output == NULL || maxLength == 0) return bytesRead;

	if (bytesWritten >= maxLength) {
		output[maxLength - 1] = '\0';
	} else {
		output[bytesWritten] = '\0';
	}

	return bytesRead;
}
