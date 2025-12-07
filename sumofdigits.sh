#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 number"
    exit 1
fi
n=$1
sum=0
while [ $n -gt 0 ]
do
    digit=$((n % 10))
    sum=$((sum + digit))
    n=$((n / 10))
done
echo "Sum of digits = $sum"
