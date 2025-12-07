#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

file=$1
if [ -f "$file" ]; then
    stat -c %y "$file"
else
    echo "File not found!"
fi

