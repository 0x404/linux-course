#! /bin/bash
# author: 1120192092-曾群鸿

# 作业1.2
#   - 求n的阶乘，n=20

result=1
for ((i=1; i<=20; ++i))
do
    result=$(expr $result \* $i)
done
echo $result