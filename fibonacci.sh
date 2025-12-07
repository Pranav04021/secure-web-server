#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 n"
    exit 1
fi
n=$1
a=0
b=1
echo "Fibonacci series up to $n terms:"
for ((i=1; i<=n; i++))
do
    echo -n "$a "
    fn=$((a + b))
    a=$b
    b=$fn
done
echo
