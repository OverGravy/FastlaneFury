import cv2
import numpy as np
import os

# Input folder
input_folder = "/home/jimmy/Documents/FastlaneFury/Bitmap"

# Define a function to remove the background and replace the original file
def remove_background_and_replace(input_image_path):
    # Read the input image
    img = cv2.imread(input_image_path)

    # Convert the image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Apply a threshold to create a binary mask
    _, thresh = cv2.threshold(gray, 1, 255, cv2.THRESH_BINARY)

    # Find contours in the binary mask
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Create a mask with the same dimensions as the input image
    mask = np.zeros_like(img)

    # Fill the contours with white color on the mask
    cv2.drawContours(mask, contours, -1, (255, 255, 255), thickness=cv2.FILLED)

    # Bitwise-AND the original image and the mask to remove the background
    result = cv2.bitwise_and(img, mask)

    # Save the result as the original file, overwriting it
    cv2.imwrite(input_image_path, result)

# Iterate through all .bmp files in the input folder
for filename in os.listdir(input_folder):
    if filename.endswith(".bmp"):
        input_image_path = os.path.join(input_folder, filename)

        # Remove the background and replace the original file
        remove_background_and_replace(input_image_path)

        print(f"Background removed from {filename} and original file replaced.")

print("Background removal and replacement complete.")
