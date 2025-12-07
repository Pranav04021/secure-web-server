#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi
file=$1
if [ ! -f "$file" ]; then
    echo "File not found!"
    exit 1
fi
lineno=1
while IFS= read -r line
do
    words=$(echo "$line" | wc -w)
    chars=$(echo "$line" | wc -c)
    echo "Line $lineno: Words=$words, Characters=$chars"
    lineno=$((lineno+1))
done < "$file"
