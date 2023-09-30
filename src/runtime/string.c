#include "include/string.h"

String *STR_new() {
	String *str = LST_new(16, 1);
	return str;
}

String *STR_from(const char *from, int len) {
	String *str = STR_new();
	for (int i = 0; i < len; i++) {
		LST_add_to_slice(str, (long) from[len]);
	}
	return str;
}
