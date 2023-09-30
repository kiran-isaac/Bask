#include "include/slice.h"

Slice *SLC_new(long length) {
	Slice *slice = malloc(sizeof(Slice));
	slice->start = malloc(sizeof(void*) * 128);
	slice->length = length;
	return slice;
}

void SLC_add_to_slice(Slice *slice, void *elem) {
	if (slice->length % 2 == 0) {
		slice->start = realloc(slice->start, slice->length * 2);
	}

	slice->start[slice->length] = elem;
	slice->length++;
}

void *SLC_index(Slice *slice, size_t index) {
	if (index >= slice->length) {
		return 0;
	}

	return slice->start[index];
}
