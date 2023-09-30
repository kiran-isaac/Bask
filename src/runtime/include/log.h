#ifndef RUN_LOG_H
#define RUN_LOG_H

#include <stdio.h>
#include <stdlib.h>

typedef enum LogType {
	LOG_E,
	LOG_W,
} LogType;

void BSK_log(LogType type, const char *err);

#endif
