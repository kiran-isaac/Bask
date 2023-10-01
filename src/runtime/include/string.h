#ifndef RUN_STRING_H
#define RUN_STRING_H

#include "list.h"

typedef List String;

String *STR_new();
String *STR_from(const char *from);

#endif
