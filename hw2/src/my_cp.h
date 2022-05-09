#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <utime.h>

#define SUCCESSID 0
#define FAILEDID 1

// 复制单个文件
int cp_single(char *src, char *tar);

// 递归复制目录
int cp_recursively(char *src, char *tar);

// 复制命令的用户接口
int my_cp(char **args);