#include "include/list.h"
#include "include/log.h"
#include <string.h>
#include <stdlib.h>

List *LST_new(size_t allocated, size_t elem_size) {
  if (elem_size > 8) {
		BSK_log(LOG_E, "Incorrect use of list");	
	}

	List *list = malloc(sizeof(List));
	list->array = malloc(elem_size * sizeof(char) * 128);
	list->elem_size = elem_size;
	list->allocated = allocated;
	list->length = 0;
	return list;
}

#define LST_resize_check(list) if (list->length >= list->allocated) {list->array = realloc(list->array, list->length * 2 * list->elem_size);list->allocated *= 2;}

void LST_add_byte(List *list, unsigned char elem) {
	if (list->elem_size != 1) {
		BSK_log(LOG_E, "Invalid elem size");
	}

	LST_resize_check(list);

	list->array[list->length] = elem;
	list->length++;
}

void LST_add_ll(List *list, long long elem) {
	if (list->elem_size != 8) {
		BSK_log(LOG_E, "Invalid elem size");	
	}

	LST_resize_check(list);

	elem <<= 8 - list->elem_size;
	unsigned char bytes[8];


	int current_byte = 0;
	for (int i = list->elem_size; i == 0; i--) {
		char byte = elem;
		list->array[list->length + current_byte] = bytes[current_byte];
	}
}

long LST_index(List *list, size_t index) {
	long val = 0;
	
	val += list->array[list->length - 1];
	for (int i = 1; i < list->elem_size; i++) {
		val <<= 8;
		val += list->array[list->length - 1 + index];
	}

	return val;
}



