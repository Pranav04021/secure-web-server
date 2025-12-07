#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 string"
    exit 1
fi
str=$1
rev=$(echo "$str" | rev)
if [ "$str" = "$rev" ]; then
    echo "$str is a palindrome"
else
    echo "$str is not a palindrome"
fi
