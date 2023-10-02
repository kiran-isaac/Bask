#include <bask/string.h>
#include <stdio.h>
#include <string.h>

int main() {
	char *test_string = "Hello World!";
	String *string = STR_from(test_string);
	if (strcmp(test_string, string->array) == 0) {
		printf("yay!");
	}
}
