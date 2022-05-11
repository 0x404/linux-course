#include "my_rm.h"
#include "utils.h"

int remove_recursively(char *dir)
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
            remove_recursively(nex);
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
        int pos = 2;
        // 遍历参数，依次进行删除，支持`rm a b c ...`多文件删除
        while (args[pos] != NULL)
        {
            // 开启`-r`选项，不是目录，使用`remove`系统调用删除
            if (!is_dir(args[pos]))
            {
                remove(args[pos]);
            }
            else
            {
                // 否则使用递归删除
                remove_recursively(args[pos]);
            }
            pos += 1;
        }
    }
    else
    {
        int pos = 1;
        // 遍历参数，依次进行删除，支持`rm a b c ...`多文件删除
        while (args[pos] != NULL)
        {
            // 不开启-r选项，检查待删除路径是否是目录
            if (is_dir(args[pos]))
            {
                printf("rm: cannot remove %s: Is a directory\n", args[pos]);
            }
            else
            {
                remove(args[pos]);
            }
            pos += 1;
        }
        
    }
    return SUCCESSID;
}