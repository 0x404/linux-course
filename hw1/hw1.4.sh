#! /bin/bash
# author: 1120192092-曾群鸿

# 作业1.4
#   - 求出一个文件中数据的最大值、最小值和平均值，精度为2
#   - shell脚本不支持浮点数，故浮点数运算使用bc计算

analyse() {
    read_count=0
    sum_value=0
    while read line
    do
        read_count=$(expr $read_count + 1)
        if [ $read_count == '1' ]
        then
            max_value=$line
            min_value=$line
        else
            # shell 逻辑运算符具有短路原则
            # 对于 expr1 && expr2，只有当expr1为真才会计算expr2
            # 利用这一原则简写计算最大值和最小值逻辑
            [ $(echo "$line > $max_value" | bc) -eq 1 ] && max_value=$line
            [ $(echo "$line < $min_value" | bc) -eq 1 ] && min_value=$line
        fi
        sum_value=$(echo "scale=2; $sum_value + $line" | bc)
    done < $1
    echo "max_value=$max_value"
    echo "min_value=$min_value"
    echo "avg_value=$(echo "scale=2; $sum_value / $read_count" | bc)"
}

analyse $@