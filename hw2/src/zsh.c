#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <utime.h>
#include <time.h>

#include "utils.h"
#include "my_rm.h"
#include "my_cp.h"

#define SUCCESSID 0
#define FAILEDID 1
#define HISTORYBUFFER 1024
#define COMMANDBUFFER 256
#define ARGUMENTBUFFER 20
#define KGRN "\x1B[32m"
#define RESET "\x1B[0m"

int my_ls(char **args);
int my_pwd(char **args);
int my_cd(char **args);
int my_mkdir(char **args);
int my_mv(char **args);
int my_history(char **args);
int my_touch(char **args);


// 当前shell支持的命令名
char *shell_funcname[] = {
    (char*)"ls",
    (char*)"pwd",
    (char*)"cd",
    (char*)"mkdir",
    (char*)"rm",
    (char*)"cp",
    (char*)"mv",
    (char*)"history",
    (char*)"touch",
};
// 当前shell支持的命令对应的函数指针
int (*shell_function[]) (char **) = {
  &my_ls,
  &my_pwd,
  &my_cd,
  &my_mkdir,
  &my_rm,
  &my_cp,
  &my_mv,
  &my_history,
  &my_touch,
};

// 循环队列，用于存储历史输入信息，最多存储HISTORYBUFFER条命令
char history[HISTORYBUFFER][COMMANDBUFFER];
int history_head = 0, history_tail = 0;


int my_ls(char **args)
{
    // 参数解析，ls命令支持`-a`选项
    if (args[1] == NULL)
    {
        args[1] = (char*)".";
        args[2] = NULL;
    }
    else if (!strcmp(args[1], "-a") && args[2] == NULL)
    {
        args[1] = (char*)".";
        args[2] = (char*)"-a";
        args[3] = NULL;
    }
    
    if (args[3] != NULL || (args[2] != NULL && strcmp(args[2], "-a")))
    {
        printf("ls: parameter error.\n");
        return FAILEDID;
    }
    
    // 利用opendir系统调用获取目录下所有文件
    DIR *dir;
    struct dirent *file;
    if (!(dir = opendir(args[1])))
    {
        printf("ls: failed in opening directory\n");
        return FAILEDID;
    }
    else
    {
        while ((file = readdir(dir)) != NULL)
        {
            if (file->d_name[0] == '.')       
            {
                // 隐藏文件，只有当`-a`选项打开时才显示
                if (args[2] == NULL || strcmp(args[2], "-a"))
                    continue;
            }
            if (is_dir(next_dir(args[1], file->d_name)))
            {
                printf(KGRN "%s  " RESET, file->d_name);
            }
            else
            {
                printf("%s  ", file->d_name);
            }
            
        }
        printf("\n");
    }
    return SUCCESSID;
}

int my_pwd(char **args)
{
    // 参数解析，pwd命令不支持任何参数
    if (args[1] != NULL)
    {
        printf("> pwd does not support any parameters.\n");
        return FAILEDID;
    }

    // 利用getcwd系统调用获取当前工作目录路径
    char work_directory[256];
    if (!getcwd(work_directory, sizeof(work_directory)))
    {
        printf("> error when get current working directory.\n");
        return FAILEDID;
    }
    printf("%s\n", work_directory);
    return SUCCESSID;
}

int my_cd(char **args)
{
    // 参数解析，cd命令有且仅有一个参数
    if (args[1] == NULL || args[2] != NULL)
    {
        printf("> cd requires only one parameter.\n");
        return FAILEDID;
    }

    // chdir系统调用成功返回0，否则返回非0
    if (chdir(args[1]))
    {
        printf("> change directory failed.\n");
        return FAILEDID;
    }
    return SUCCESSID;
}

int my_mkdir(char **args)
{
    // 参数解析，mkdir至少需要一个参数
    if (args[1] == NULL)
    {
        printf("mkdir: missing operand\n");
        return FAILEDID;
    }
    
    int pos = 1, failed = 0;
    while (args[pos] != NULL)
    {
        // 使用mkdir系统调用创建目录，并进行错误处理
        if (mkdir(args[pos], S_IRWXU) == -1)
        {
            if (errno == EEXIST)
            {
                // 待创建目录已经存在
                printf("mkdir: %s directory already exists.\n", args[pos]);
            }
            else if (errno == ENAMETOOLONG)
            {
                // 待创建目录名过长
                printf("mkdir: %s directory name too long.\n", args[pos]);
            }
            else
            {
                printf("mkdir: make %s failed.\n", args[pos]);
            }
            failed = 1;
        }
        pos += 1;
    }
    if (failed) return FAILEDID;
    return SUCCESSID;
}


int my_mv(char **args)
{
    // 参数检查，仅支持`mv source target`
    if (args[1] == NULL || args[2] == NULL)
    {
        printf("mv: requires source path and target path\n");
    }
    else if (args[3] != NULL)
    {
        printf("mv: unkown parameter %s", args[3]);
    }

    if (is_dir(args[2]))
    {
        args[2] = next_dir(args[2], args[1]);
    }

    if (is_dir(args[1]))
    {
        // 如果源文件是目录，调用`my_cp -r src tar`
        for (int i = 4; i >= 2; --i)
            args[i] = args[i - 1];
        args[1] = (char*)"-r";
        my_cp(args);

        // 调用`my_rm -r src`
        args[3] = NULL;
        my_rm(args);
    }
    else if (is_file(args[1]))
    {
        // 如果源文件是文件，调用`my_cp src tar`
        my_cp(args);

        // 调用`my_rm src`
        args[2] = NULL;
        my_rm(args);
    }
    else
    {
        printf("mv: %s not exist\n", args[1]);
        return FAILEDID;
    }
    return SUCCESSID;
}

int my_history(char **args)
{
    int position = history_head, counter = 1;
    while (position != history_tail)
    {
        printf("%4d %s\n", counter, history[position]);
        position = (position + 1) % HISTORYBUFFER;
        counter += 1;
    }
    return SUCCESSID;
}

void update_history(char *line)
{
    // 将新的输入命令添加到历史缓冲区的尾部
    int length = strlen(line);
    strcpy(history[history_tail], line);
    if (history[history_tail][length - 1] == '\n')
    {
        history[history_tail][length - 1] = '\0';
    }
    
    // 尾部指针加1
    history_tail = (history_tail + 1) % HISTORYBUFFER;

    // 历史缓冲区已满，删除最早的记录
    if (history_head == history_tail)
    {
        history_head = (history_head + 1) % HISTORYBUFFER;
    }

}

int my_touch(char **args)
{
    // 新增命令，方便在终端中测试`cp`，`mv`， `rm`命令
    // 支持touch a b c d ...，创建多个文件
    if (args[1] == NULL)
    {
        printf("touch: missing file operand\n");
        return FAILEDID;
    }

    int pos = 1;
    while (args[pos] != NULL)
    {
        if (is_file(args[pos]))
        {
            printf("touch: %s exists\n", args[pos]);
        }
        else
        {
            int file = creat(args[pos], 0700);
            close(file);
        }
        pos += 1;
    }
    return SUCCESSID;
}

void zsh()
{
    // 程序入口
    while (1)
    {
        printf("root@zsh:$ ");

        // step1 读取用户输入
        char *line = read_line();
        if (line == NULL) continue;

        // step2 更新用户命令历史信息
        update_history(line);

        // step3 将输入转成参数形式
        char **args = parse_line(line);

        // step4 遍历当前shell支持的命令，查找用户命令
        int processed = 0;
        int zsh_func_num = sizeof(shell_funcname) / sizeof(char*);
        for (int i = 0; i < zsh_func_num; ++i)
        {
            if (!strcmp(args[0], shell_funcname[i]))
            {
                // 查找到用户命令，通过函数指针调用对应命令执行
                processed = 1;
                int ret = (*shell_function[i])(args);
                break;
            }
        }

        if (!strcmp(args[0], "exit"))
        {
            break;
        }

        // step5 用户命令未找到，输出提示信息
        if (!processed) printf("%s: command not found\n", args[0]);
    }
}

int main()
{
    zsh();
    return 0;
}