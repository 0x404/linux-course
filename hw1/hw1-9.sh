#! /bin/bash

###
 # @author: 1120192092-曾群鸿
 # @Homework1-9:
 #  - 编写一个名为chname的程序，将当前目录下所有的.txt文件更名为.doc文件
 #  - 使用说明：
 #          - hw1-9.sh  （将当前目录下所有txt文件该为doc文件）
### 

for file in $(ls *.txt)
do
    # 使用basename获取文件去掉扩展名后的结果
    BASENAME=$(basename $file .txt)
    mv "$BASENAME.txt" "$BASENAME.doc"
done