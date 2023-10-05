#ifndef RUN_LIST_H
#define RUN_LIST_H

#include <stdlib.h>

// Max elem size is 8
typedef struct List {
	size_t length;
	size_t allocated;
	size_t elem_size;
	char *array;
} List;

List *LST_new(size_t, size_t);
void LST_add_byte(List *, unsigned char);
void LST_add_int(List *, unsigned int);
void LST_add_ll(List *, long long);
void LST_list(List *, unsigned int);
long LST_index(List *, size_t);
void LST_free(List *);

#endif
