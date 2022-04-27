#! /bin/bash

if [ $# != 1 ]
then
    echo "file-finder: argument error. use file-find -h to get help"
elif [ $1 == "-h" ]
then
    echo "file-finder str (list all file in current directory with \"str\")"
    echo "file-finder -h  (display help document)"
    exit 0
fi

ok=0
for file in `ls`
do
    if [[ $file == *$1* ]]
    then
        echo $file
        ok=1
    fi
done

if [ $ok == 0 ]
then
    echo "[find-finder]: find nothing."
fi