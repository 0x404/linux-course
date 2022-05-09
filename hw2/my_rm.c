#include "my_rm.h"
#include "utils.h"

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