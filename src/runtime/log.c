#include "include/log.h"

#define Color_Yellow  "\33[0:33m\\]"
#define Color_Red "\33[0:31m\\]"
#define Color_end "\33[0m\\]" // To flush out prev settings
#define LOG_RED(X) 
void BSK_log(LogType type, const char *err) {
	switch (type) {
		case LOG_E:
			printf("%s %s %s",Color_Red,err,Color_end);
			exit(1);
		case LOG_W:
			printf("%s %s %s",Color_Yellow,err,Color_end);
	}
}
