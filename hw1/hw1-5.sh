#! /bin/bash

###
 # @author: 1120192092-曾群鸿
 # @Homework1-5:
 #  - 编写程序在目录的所有文件中查找包含指定字符串或者指定日期的文件名。
 #  - 使用说明：
 #          - hw1-5.sh PATH STR     （在路径PATH下查找包含STR的文件名）
 #          - hw1-5.sh -h            (显示帮助信息)
### 

# 检查参数是否合法
if [ $# == 0 ]
then
    echo "[hw1-5.sh]: argument error."
    echo "[hw1-5.sh]: use -h to get help document."
    exit 1
fi

if [ $1 == "-h" ]
then
    # 显示帮助信息
    echo "hw1-5.sh  PATH STR    (list all file in PATH with \"STR\")"
    echo "hw1-5.sh  -h          (display help document)"
    exit 0
elif [ $# != 2 ]
then
    echo "[hw1-5.sh]: argument error."
    echo "[hw1-5.sh]: use -h to get help document."
    exit 1
fi

ok=0
for file in `ls $1`
do
    # 使用通配符判断一个字符串是否包含某子串
    if [[ $file == *$2* ]]
    then
        echo $file
        ok=1
    fi
done

if [ $ok == 0 ]
then
    # 如果没有找到任何文件，则输出find nothing
    echo "[hw1-5.sh]: find nothing."
fi