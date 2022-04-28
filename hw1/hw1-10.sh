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
    for u in $(who | awk '{print $1}' | sort | uniq)
    do
        if [ $u == $USERNAME ]
        then
            echo "$USERNAME is logged in."
            find=1
        fi
    done
    [ $find == 0 ] && echo "$USERNAME is not logged yet."
    sleep 300
done