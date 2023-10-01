#include <bask/string.h>
#include <bask/list.h>
#include <string.h>

String *STR_new() {
	String *str = LST_new(16, 1);
	return str;
}

String *STR_from(const char *from) {
	int len = strlen(from);
	String *str = STR_new();
	for (int i = 0; i < len; i++) {
		LST_add_to_slice(str, (long) from[i]);
	}
	return str;
}
