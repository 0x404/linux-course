#! /bin/bash
# author: 1120192092-曾群鸿

# 作业1.1
#   - 建立路径./a，./b，在./a目录下建立一些文件，*.c；
#   - 利用for循环将当前目录下的（*.c）文件移到指定的目录（如./b）下
#     并按文件大小排序，显示移动后指定目录的内容。


# 建立./a ./b目录（如果已经存在则直接使用）
mkdir -p ./a ./b

# 随机生成一个指定长度为的字符串
rand_name() {
    name=$(date +%s%N | md5sum | head -c $1)
    echo $name
    return 0
}

# 在a目录下随机生成[5, 14]个文件
file_count=$(expr $RANDOM % 10 + 5)
for ((i=1; i<=$file_count; ++i))
do
    file_name="a/$(rand_name 5).c"
    file_size=$(expr $RANDOM % 30)
    touch $file_name
    echo "this is the content of $(rand_name $file_size)" >> $file_name
done

# 用for循环遍历a目录下的文件，将*.c文件移动到b目录
for file in $(ls a)
do
    # 如果当前文件为*.c文件，移动到b文件夹
    if [[ $file == *.c ]]
    then
        mv a/$file b
    fi
done

# 输出b目录下按文件大小从大到小排序的结果
echo "cotent of directory ./b (sorted by file size, largest first):"
ls -S b

