#!/bin/bash
count=0
while [ $count -lt 3 ]; do
    echo "Process list at $(date):"
    ps -e
    count=$((count+1))
    sleep 30
done
