#!/bin/bash
if [ $# -eq 0 ]; then
    echo "Usage: $0 files..."
    exit 1
fi

for file in "$@"; do
    newname=$(echo "$file" | tr 'A-Z' 'a-z')
    mv "$file" "$newname"
    echo "Renamed $file -> $newname"
done
