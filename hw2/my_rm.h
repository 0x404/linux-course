#include <unistd.h>
#include <dirent.h>

#define SUCCESSID 0
#define FAILEDID 1

int rmmove_recursively(char *dir);
int my_rm(char **args);