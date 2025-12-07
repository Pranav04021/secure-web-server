#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 number"
    exit 1
fi
n=$1
fact=1
for ((i=1; i<=n; i++))
do
    fact=$((fact * i))
done
echo "Factorial of $n is $fact"
