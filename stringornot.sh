#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 input"
    exit 1
fi
input=$1
if [[ $input =~ ^[0-9]+$ ]]; then
    echo "$input is a number"
else
    echo "$input is a string"
fi
