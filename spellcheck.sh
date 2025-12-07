#!/bin/bash
# Check if a filename is provided as an argument
if [ -z "$1" ]; then
echo "Usage: $0 <textfile>"
exit 1
fi
# Check if the file exists
if [ ! -f "$1" ]; then
echo "The file '$1' does not exist."
exit 1
fi
# Check the spelling of the file using aspell
echo "Checking spelling for file: '$1'"
aspell check "$1"
