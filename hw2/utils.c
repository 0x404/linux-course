#include "utils.h"


int is_dir(char *path)
{
    // 通过文件的stat检查是否是目录
    struct stat fileinfo;
    stat(path, &fileinfo);
    if (S_ISDIR(fileinfo.st_mode)) return 1;
    return 0;
}

int is_file(char *path)
{
    struct stat fileinfo;
    return (stat(path, &fileinfo) == 0);
}

char *next_dir(char *current, char *nex)
{
    // 字符串拼接
    char *res = (char*)malloc(256 * sizeof(char));
    strcpy(res, current);
    strcat(res, "/");
    strcat(res, nex);
    return res;
}

char *read_line()
{
    // 从系统stdin中读取一行
    char *buffer = (char*)malloc(COMMANDBUFFER * sizeof(char));
    int pos = 0, c = 0;

    if (buffer == NULL)
    {
        printf("zsh: something went wrong when reading input from stdin\n");
        return NULL;
    }

    // 不断读取输入字符，直到遇到回车或者EOF
    while (1)
    {
        c = getchar();
        if (c == '\n')
        {
            buffer[pos] = c;
            return buffer;
        }
        else if (c == EOF)
        {
            return buffer;
        }
        else
        {
            buffer[pos++] = c;
        }

        if (pos == COMMANDBUFFER)
        {
            printf("zsh: command too long\n");
            return NULL;
        }
    }
}


char **parse_line(char *line)
{
    // 利用strtok将给定字符串分割成若干token作为参数
    const char *delim = " \t\n";
    char **args = (char **)malloc(ARGUMENTBUFFER * sizeof(char*));
    char *arg;
    int pos = 0;

    arg = strtok(line, delim);
    while (arg != NULL)
    {
        args[pos++] = arg;
        arg = strtok(NULL, delim);
    }
    args[pos] = NULL;
    return args;
}