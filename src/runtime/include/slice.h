#ifndef RUN_SLICE_H
#define RUN_SLICE_H

#include <stdlib.h>

typedef struct Slice {
  long length;
	void **start;
} Slice;

Slice *SLC_slice(long length);
void SLC_add_to_slice(Slice *slice, void *elem);
void *SLC_index(Slice *slice, size_t index);

#endif
