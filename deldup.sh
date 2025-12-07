#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage: $0 dir1 dir2"
    exit 1
fi

dir1=$1
dir2=$2

for file in "$dir1"/*; do
    fname=$(basename "$file")
    if [ -f "$dir2/$fname" ] && cmp -s "$file" "$dir2/$fname"; then
        echo "Deleting duplicate file: $file"
        rm "$file"
    fi
done
