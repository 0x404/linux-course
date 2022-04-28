#! /bin/bash
# author: 1120192092-曾群鸿

# 作业1.3
#   - for循环打印下面这句话中字母数不大于6的单词。
#     I am oldboy teacher welcome to oldboy training class

sentence="I am oldboy teacher welcome to oldboy training class"
sentence=($sentence)
sentence_len=${#sentence[*]}

for ((i=0; i<$sentence_len; ++i))
do
    if [[ ${#sentence[i]} -le 6 ]]
    then
        echo ${sentence[i]}
    fi
done
