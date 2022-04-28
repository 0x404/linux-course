#! /bin/bash

###
 # @author: 1120192092-曾群鸿
 # @Homework1-7:
 #  - 编写一个名为ifuser的程序，它执行时带用户名作为命令行参数，
 #    判断该用户是否已经在系统中登录，并给出相关信息。
 #  - 使用说明：
 #          - hw1-7.sh  USER    （查询USER是否登录，给出相关信息）
### 


# 检查输入参数，如果不合法输出帮助信息
[ $# == 1 ] || {
    echo "[hw1-7]: argument error."
    echo "[hw1-7]: use $0 USER to get USER info."
    exit 1
}

# who命令显示当前系统登录用户
# 格式为：zengqunhong   pts/1   2022-04-21  08:04   (tmux(999224).%0)
# 使用awk获得第一列的用户名，然后排序去重，得到当前系统登录的全部用户名
find=0
for u in $(who | awk '{print $1}' | sort | uniq)
do
    if [ $u == $1 ]
    then
        find=1
        echo "$1 is logged in."
    fi
done
[ $find == 0 ] && echo "$1 is not logged in."
