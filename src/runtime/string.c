#include "include/string.h" 
#include "include/list.h"
#include <string.h>

String *STR_new() {
	String *str = LST_new(16, 1);
	return str;
}

String *STR_from(const char *from) {
	int len = strlen(from) + 1;
	String *str = STR_new();
	for (int i = 0; i < len; i++) {
		LST_add_byte(str, (long) from[i]);
	}
	return str;
}
