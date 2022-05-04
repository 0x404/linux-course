#! /bin/bash

###
 # @author: 1120192092-曾群鸿
 # @Homework1-2:
 #  - 求n的阶乘，n=20
 #  - 使用说明：
 #          - hw1-2.sh  [n]   (计算n的阶乘，如果没有给出n则计算20的阶乘)
### 

n=20
[ $# == 1 ] && n=$1 # 如果指定参数，则按照参数进行计算

result=1
for ((i=1; i<=n; ++i))
do
    result=$(expr $result \* $i)
done
echo $result