from PIL import Image
import os

# Input and output folders
input_folder = "/home/jimmy/Documents/FastlaneFury/Bitmap"
output_folder = "/home/jimmy/Documents/FastlaneFury/Bitmap"

# Create the output folder if it doesn't exist
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Iterate through all PNG files in the input folder
for filename in os.listdir(input_folder):
    if filename.endswith(".png"):
        input_image_path = os.path.join(input_folder, filename)
        output_image_path = os.path.join(output_folder, os.path.splitext(filename)[0] + ".bmp")

        # Open the PNG image with transparency
        png_image = Image.open(input_image_path)

        # Convert and save the image as BMP with specific options
        png_image.save(output_image_path, "BMP", params=['-d', 'n', '-r', 'n', '-s', 'n'])

        # Delete the original PNG file
        os.remove(input_image_path)

        print(f"{filename} converted to BMP with options and original PNG file deleted.")

print("Conversion and deletion complete.")
