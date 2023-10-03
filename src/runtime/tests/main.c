#include <bask/string.h>
#include <stdio.h>
#include <string.h>

void string_tests() {
	char *test_string = "Hello World! This is a long long string";
	String *string = STR_from(test_string);
	if (strcmp(test_string, string->array) != 0) {
		printf("yay!");
	}
	LST_free(string);
}

void list_tests() {
	List *list = LST_new(1, 2);

	LST_add_int(list, 10);
}

int main() {
	string_tests();
	list_tests();
}
