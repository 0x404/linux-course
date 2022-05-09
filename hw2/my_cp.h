#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <utime.h>

#define SUCCESSID 0
#define FAILEDID 1

int cp_single(char *src, char *tar);
int cp_recursively(char *src, char *tar);
int my_cp(char **args);