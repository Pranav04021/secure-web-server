#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage: $0 inputfile outputfile"
    exit 1
fi

tr -d ' ' < "$1" > "$2"
echo "Spaces removed. Output saved to $2"
