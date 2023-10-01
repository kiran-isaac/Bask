#include "include/list.h"
#include "include/log.h"

List *LST_new(size_t length, size_t elem_size) {
  if (elem_size > 8) {
		BSK_log(LOG_E, "Incorrect use of list");	
	}

	List *list = malloc(sizeof(List));
	list->array = malloc(elem_size * sizeof(char) * 128);
	list->elem_size = elem_size;
	list->length = length;
	return list;
}


void LST_add_to_slice(List *list, long elem) {
	if (list->length % 2 == 0) {
		list->array = realloc(list->array, list->length * 2 * list->elem_size);
	}
	
	elem <<= 8 - list->elem_size;
	unsigned char bytes[8];
	convert_to_char_array(bytes, elem);

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



