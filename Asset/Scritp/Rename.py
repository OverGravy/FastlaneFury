import os

# Folder path where the files to be renamed are located
folder = '/home/jimmy/Documents/FastlaneFury/Bitmap'

# Word to add to the new file names
word_to_add = 'veicle'

# Initialize a counter for the number
counter = 0

# Iterate through all the files in the folder
for filename in os.listdir(folder):
    if os.path.isfile(os.path.join(folder, filename)):
        # Get the file extension
        extension = os.path.splitext(filename)[1]

        # New file name
        new_filename = f'{word_to_add}{counter}{extension}'

        # Rename the file
        old_path = os.path.join(folder, filename)
        new_path = os.path.join(folder, new_filename)
        os.rename(old_path, new_path)

        # Increment the counter
        counter += 1

print('Renaming completed!')