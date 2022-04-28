#! /bin/bash

###
 # @author: 1120192092-曾群鸿
 # @Homework1-6:
 #  - 编写一个名为greet的问候程序，它执行时能根据系统当前的时间向用户输出问候信息。
 #    设从半夜到中午为早晨，中午到下午六点为下午，下午六点到半夜为晚上。
 #  - 使用说明：
 #          - hw1-6.sh  （输出问候信息）
### 

current=$(date +%H)
if [ $current -ge 0 -a $current -lt 12 ]
then
    greet="good morning"
elif [ $current -ge 12 -a $current -lt 18 ]
then
    greet="good afternoon"
else
    greet="good eveing"
fi

echo "$greet! $USER, today is $(date +%A)."
# 输出：good morning! zengqunhong, today is Thursday.
