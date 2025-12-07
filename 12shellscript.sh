#!/bin/bash
if [ $# -eq 0 ]; then
    echo "Usage: $0 string"
    exit 1
fi

str=$1
echo "Length of string '$str' is: ${#str}"
