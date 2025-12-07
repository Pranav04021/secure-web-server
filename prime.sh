#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 number"
    exit 1
fi
n=$1
if [ $n -le 1 ]; then
    echo "$n is not a prime number"
    exit 0
fi
for ((i=2; i<=n/2; i++))
do
    if [ $((n % i)) -eq 0 ]; then
        echo "$n is not a prime number"
        exit 0
    fi
done
echo "$n is a prime number"
