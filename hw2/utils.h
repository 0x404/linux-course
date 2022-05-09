#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

#define COMMANDBUFFER 128
#define ARGUMENTBUFFER 20

// 判断给定路径是否是一个linux目录
int is_dir(char *path);

// 判断给定路径是否是一个linux文件
int is_file(char *path);

// 生成一个类似`current/nex`的路径字符串
char *next_dir(char *current, char *nex);

// 从stdin中读取一行
char *read_line();

// 将一个字符串通过空格换行符切分成多个token
char **parse_line(char *line);