/*
 * @author: 1120192092-曾群鸿
 * @Date: 2022-05-11 19:42:19
 * @Description: rm命令所需要函数的定义，更多信息请见`./readme.md`。
 */
#include <unistd.h>
#include <dirent.h>

#define SUCCESSID 0
#define FAILEDID 1

// 递归删除目录
int remove_recursively(char *dir);

// 删除命令的用户接口
int my_rm(char **args);