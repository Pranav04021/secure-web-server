#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

# Step 1: Remove punctuation, make lowercase
tr -cs 'A-Za-z' '[\n*]' < "$1" | tr 'A-Z' 'a-z' > temp_words.txt

# Step 2: Sort and unique
sort temp_words.txt | uniq > wordlist.txt
rm temp_words.txt

echo "Wordlist saved as wordlist.txt"
