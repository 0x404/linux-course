#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

#include <fcntl.h>
#include <utime.h>

#define KGRN "\x1B[32m"
#define RESET "\x1B[0m"
#define SUCCESSID 0
#define FAILEDID 1
#define HISTORYBUFFER 1024
#define COMMANDBUFFER 128
#define ARGUMENTBUFFER 20

int my_ls(char **args);
int my_pwd(char **args);
int my_cd(char **args);
int my_mkdir(char **args);
int my_rm(char **args);
int my_cp(char **args);
int my_mv(char **args);
int my_history(char **args);

char *shell_funcname[] = {
    (char*)"ls",
    (char*)"pwd",
    (char*)"cd",
    (char*)"mkdir",
    (char*)"rm",
    (char*)"cp",
    (char*)"mv",
    (char*)"history",
};
int (*shell_function[]) (char **) = {
  &my_ls,
  &my_pwd,
  &my_cd,
  &my_mkdir,
  &my_rm,
  &my_cp,
  &my_mv,
  &my_history,
};

char history[HISTORYBUFFER][COMMANDBUFFER];
int history_head = 0, history_tail = 0;

int is_dir(char *path)
{
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
    char *res = (char*)malloc(256 * sizeof(char));
    strcpy(res, current);
    strcat(res, "/");
    strcat(res, nex);
    return res;
}


char *read_line()
{
    char *buffer = (char*)malloc(COMMANDBUFFER * sizeof(char));
    int pos = 0, c = 0;

    if (buffer == NULL)
    {
        printf("zsh: something went wrong when reading input from stdin\n");
        return NULL;
    }

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
    // 参数解析，mkdir命令有且仅有一个参数
    if (args[1] == NULL || args[2] != NULL)
    {
        printf("> mkdir requires only one parameter.\n");
        return FAILEDID;
    }

    // 使用mkdir系统调用创建目录，并进行错误处理
    if (mkdir(args[1], S_IRWXU) == -1)
    {
        if (errno == EEXIST)
        {
            // 待创建目录已经存在
            printf("> directory already exists.\n");
        }
        else if (errno == ENAMETOOLONG)
        {
            // 待创建目录名过长
            printf("> directory name too long.\n");
        }
        else
        {
            printf("> mkdir failed.\n");
        }
        return FAILEDID;
    }
    return SUCCESSID;
}


int rmmove_recursively(char *dir)
{
    DIR *current_dir;
    struct dirent *file;
    if (!(current_dir = opendir(dir)))
    {
        printf("> failed in rm directory\n");
        return FAILEDID;
    }
    while (file = readdir(current_dir))
    {
        // 跳过特殊文件
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
        {
            continue;
        }

        // 获取下一个文件路径
        char *nex = next_dir(dir, file->d_name);

        if (is_dir(nex))
        {
            // 目录文件，递归删除
            rmmove_recursively(nex);
        }
        else
        {
            // 普通文件，使用remove系统调用删除
            remove(nex);
        }
    }
    remove(dir);
    return SUCCESSID;
}

int my_rm(char **args)
{
    // 参数解析，支持`rm file`和`rm -r file`
    if (args[1] == NULL || (!strcmp(args[1], "-r") && args[2] == NULL))
    {
        printf("rm: requires file path parameter.\n");
        return FAILEDID;
    }
    
    if (!strcmp(args[1], "-r"))
    {
        if (!is_dir(args[2]))
        {
            // 开启-r选项但不是目录，使用remove删除
            remove(args[2]);
        }
        else
        {
            // 开启-r选项且是目录，递归删除
            rmmove_recursively(args[2]);
        }
    }
    else
    {
        // 不开启-r选项，检查待删除路径是否是目录
        if (is_dir(args[1]))
        {
            printf("rm: cannot remove %s: Is a directory\n", args[1]);
            return FAILEDID;
        }
        remove(args[1]);
    }
    return SUCCESSID;
}

int cp_single(char *src, char *tar)
{
    // 打开源文件
    int src_filed = open(src, O_RDONLY);
    if (src_filed == -1)
    {
        printf("cp: open %s failed\n", src);
        return FAILEDID;
    }

    // 获取源文件信息
    struct stat file_src;
    stat(src, &file_src);

    // 创建目标文件
    int tar_filed = creat(tar, file_src.st_mode);
    if (tar_filed == -1)
    {
        printf("cp: create %s failed\n", tar);
        return FAILEDID;
    }

    // 读取源文件，写入目标文件
    int size1, size2;
    char buffer[4096];
    while ((size1 = read(src_filed, buffer, 4096)) > 0)
    {
        size2 = write(tar_filed, buffer, size1);
        if (size1 != size2)
        {
            printf("cp: write %s to %s failed\n", src, tar);
            return FAILEDID;
        }
    }

    // 设置目标文件时间属性，与源文件保持一致
    struct utimbuf utime_info;
    utime_info.actime = file_src.st_atime;
    utime_info.modtime = file_src.st_mtime;
    utime(tar, &utime_info);

    close(src_filed);
    close(tar_filed);
    return SUCCESSID;
}

int cp_recursively(char *src, char *tar)
{
    DIR *current_dir = opendir(src);
    struct dirent *file;
    while (file = readdir(current_dir))
    {
        // 遍历当前目录下所有文件
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
        {
            continue;
        }

        char *nex = next_dir(src, file->d_name);

        if (is_dir(nex))
        {
            // 如果当前文件是目录
            char *src_nex = next_dir(src, file->d_name);
            char *tar_nex = next_dir(tar, file->d_name);
            DIR *nex_tar_dir = opendir(tar_nex);
            if (nex_tar_dir == NULL)
            {
                // 创建目标目录
                struct stat src_file_info;
                struct utimbuf utime_info;

                stat(src_nex, &src_file_info);
                mkdir(tar_nex, src_file_info.st_mode);
            }
            // 递归复制当前目录
            cp_recursively(src_nex, tar_nex);

            // 回溯时用源目录信息更新目标目录时间信息
            struct stat src_file_info;
            struct utimbuf utime_info;
            stat(src_nex, &src_file_info);
            utime_info.actime = src_file_info.st_atime;
            utime_info.modtime = src_file_info.st_mtime;
            utime(tar_nex, &utime_info);
        }
        else
        {
            // 如果当前文件是普通文件
            char *src_nex = next_dir(src, file->d_name);
            char *tar_nex = next_dir(tar, file->d_name);
            // 调用cp_single进行普通文件的复制
            if (cp_single(src_nex, tar_nex) == FAILEDID)
                return FAILEDID;
        }
    }
    return SUCCESSID;
}

int my_cp(char **args)
{
    // 参数检查，支持`cp source target`和`cp -r source target`
    if (args[1] == NULL || args[2] == NULL)
    {
        printf("cp: requires source path and target path\n");
        return FAILEDID;
    }
    if (!strcmp(args[1], "-r") && (args[2] == NULL || args[3] == NULL))
    {
        printf("cp: requires source path and target path\n");
        return FAILEDID;
    }

    // 开启`-r`递归复制选项
    if (!strcmp(args[1], "-r"))
    {
        if (!is_dir(args[2]))
        {
            // 开启`-r`选项，但不是目录，单文件复制
            cp_single(args[2], args[3]);
        }
        else
        {
            // 开启`-r`选项，且是目录，递归复制
            if (opendir(args[3]) == NULL)
            {
                struct stat src_file_info;
                stat(args[2], &src_file_info);
                mkdir(args[3], src_file_info.st_mode);
            }

            cp_recursively(args[2], args[3]);

            // 递归复制完成后，需要对最外层的目录同步源目录时间信息
            struct utimbuf utime_info;
            struct stat src_file_info;
            stat(args[2], &src_file_info);
            utime_info.actime = src_file_info.st_atime;
            utime_info.modtime = src_file_info.st_mtime;
            utime(args[3], &utime_info);
        }
    }
    else
    {
        // 不开启`-r`选项，检查待复制文件是否为目录
        if (is_dir(args[1]))
        {
            printf("cp: cannot copy %s: Is a directory\n", args[1]);
            return FAILEDID;
        }
        else
        {
            cp_single(args[1], args[2]);
        }
    }
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

void zsh()
{
    while (1)
    {
        printf("root@zsh:$ ");
        char *line = read_line();
        if (line == NULL) continue;
        update_history(line);

        char **args = parse_line(line);
        int processed = 0;
        int zsh_func_num = sizeof(shell_funcname) / sizeof(char*);
        for (int i = 0; i < zsh_func_num; ++i)
        {
            if (!strcmp(args[0], shell_funcname[i]))
            {
                processed = 1;
                int ret = (*shell_function[i])(args);
                break;
            }
        }

        if (!strcmp(args[0], "exit"))
        {
            break;
        }
        if (!processed) printf("%s: command not found\n", args[0]);
    }
}

int main()
{
    zsh();
    return 0;
}