#!/bin/bash
echo "Enter a string:"
read str

if [ ${#str} -lt 5 ]; then
    echo "String must have at least 5 characters (including symbols)."
else
    echo "Valid string: $str"
fi
