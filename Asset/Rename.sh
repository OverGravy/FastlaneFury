#!/bin/bash

# Directory path where the files are located
directory="/home/jimmy/Documents/FastlaneFury/Bitmap"

# Word to add to the new file names
word_to_add="bitmap"

# Initialize a counter for the number
counter=0

# Iterate through all files in the directory
for file in "$directory"/Bitmap*.bmp; do
    if [ -f "$file" ]; then
        # New file name
        new_filename="${word_to_add}${counter}.bmp"

        # Rename the file
        mv "$file" "$directory/$new_filename"

        counter=$((counter + 1))
        echo "renamed"
    fi
done

echo "Renaming completed!"
