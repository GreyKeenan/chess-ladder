#ifndef MAIN_utilities
#define MAIN_utilities


#include <stdlib.h>
#include <stdio.h>


#define whereami() fprintf(stderr, "\n@--%s/%s()#%d\n", __FILE__, __func__, __LINE__);
#define sneeze(...) { whereami() fprintf(stderr, __VA_ARGS__); }
#define panic(...) { sneeze(__VA_ARGS__); exit(1); }


#endif
