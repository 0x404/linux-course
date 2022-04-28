#! /bin/bash

###
 # @author: 1120192092-曾群鸿
 # @Homework1-8:
 #  - 编写一个名为menu的程序，实现简单的弹出式菜单功能，
 #    用户能根据显示的菜单项从键盘选择执行对应的命令。
 #  - 使用说明：
 #          - hw1-8.sh  （显示菜单，用户菜单输入选项执行命令）
### 

menu () {
    echo "Selec the command you want to execute"
    echo "1) ls"
    echo "2) date"
    echo "3) pwd"
    echo "4) top"
    echo "5) just quit"
}

while :
do
    menu
    read opt
    case $opt in
        1)
        echo "executing ls ..."
        ls
        ;;
        2)
        echo "executing date ..."
        date
        ;;
        3)
        echo "executing pwd ..."
        pwd
        ;;
        4)
        echo "executing top ..."
        top
        ;;
        5)
        exit 0
        ;;
        *) echo "invalid option, please try again";;
    esac
done


