#ifndef MAIN_utilities
#define MAIN_utilities


#include <stdlib.h>
#include <stdio.h>


#define sneeze(...) { fprintf(stderr, "\nSNEEZE {%s}, {%s}, {%d}\n", __FILE__, __func__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); }

#define panic(...) { sneeze(__VA_ARGS__); exit(1); }


#endif
