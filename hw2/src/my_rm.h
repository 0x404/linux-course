#include <unistd.h>
#include <dirent.h>

#define SUCCESSID 0
#define FAILEDID 1

// 递归删除目录
int rmmove_recursively(char *dir);

// 删除命令的用户接口
int my_rm(char **args);