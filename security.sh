#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

tr 'A-Za-z' 'D-ZA-Cd-za-c' < "$1" > "$1.enc"
echo "Encrypted file saved as $1.enc"
