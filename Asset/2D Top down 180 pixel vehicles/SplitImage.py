import cv2
import os

# Create a folder named "Vehicle" if it doesn't already exist
if not os.path.exists("Bitmap"):
    os.mkdir("Bitmap")

# Load the big bitmap image
image = cv2.imread('/home/jimmy/Documents/FastlaneFury/Asset/2D Top down 180 pixel vehicles/AllVeicle4K.png', cv2.IMREAD_UNCHANGED)

if image is None:
    print("Failed to load the image.")
else:
    # Split the alpha channel (transparency channel)
    b, g, r, alpha = cv2.split(image)

    # Find contours in the alpha channel
    contours, _ = cv2.findContours(alpha, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Create individual bitmaps for each object
    object_count = 0
    for contour in contours:
        # Get the bounding box of the object
        x, y, w, h = cv2.boundingRect(contour)

        # Create a new bitmap with a transparent background, preserving original dimensions
        object_bitmap = image[y:y + h, x:x + w]

        # Save the individual object as a PNG with a transparent background in the "Vehicle" folder
        filename = os.path.join("Bitmap", f'veicle{object_count}.png')
        cv2.imwrite(filename, object_bitmap)
        object_count += 1
