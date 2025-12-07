#!/bin/bash
if [ $# -eq 0 ]; then
    echo "Usage: $0 num1 num2 num3 ..."
    exit 1
fi
sum=0
count=$#
for num in "$@"
do
    sum=$((sum + num))
done
avg=$(echo "scale=2; $sum / $count" | bc)
echo "Average = $avg"
