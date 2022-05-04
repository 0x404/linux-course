#! /bin/bash

###
 # @author: 1120192092-曾群鸿
 # @Homework1-10:
 #  - 编写一个名为chuser的程序，执行中每隔5分钟检查指定的用户是否登录系统，用户名从命令行输入；
 #  - 如果指定的用户已经登录，则显示相关信息。
 #  - 使用说明：
 #          - hw1-10.sh  （打开程序，输入用户名）
### 

echo "enter username to check:"
read USERNAME
while :
do
    find=0
    # who命令显示当前系统登录用户
    # 格式为：zengqunhong   pts/1   2022-04-21  08:04   (tmux(999224).%0)
    # 使用awk获得第一列的用户名，然后排序去重，得到当前系统登录的全部用户名
    # 在当前系统登录的全部用户名中查找指定用户名
    for u in $(who | awk '{print $1}' | sort | uniq)
    do
        if [ $u == $USERNAME ]
        then
            echo "$USERNAME is logged in."
            find=1
        fi
    done
    [ $find == 0 ] && echo "$USERNAME is not logged yet."
    sleep 300   # 睡眠5分钟后再次检查
done