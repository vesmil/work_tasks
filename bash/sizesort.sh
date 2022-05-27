#!/bin/bash

directory=$1

if [ ! -d "${directory}" ] ; then
    echo "\"${directory}\" is not a valid folder";
    exit 1
fi

cd "${directory}"

decimals=$(($(ls | wc -l | wc -c) - 1))

count=1

for file in $(ls -S)
do
    new_name=$(printf "%0${decimals}d" ${count})_${file}
    mv "${file}" "${new_name}"
    ((count++))
done
