#ifndef RUN_LIST_H
#define RUN_LIST_H

#include <stdlib.h>

// Max elem size is 8
typedef struct List {
	size_t length;
	size_t elem_size;
	char *array;
} List;

List *LST_new(size_t, size_t);
void LST_add_to_slice(List *, long);
long LST_index(List *, size_t);

#endif
