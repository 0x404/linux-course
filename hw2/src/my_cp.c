#include "my_cp.h"
#include "utils.h"

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