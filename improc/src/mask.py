import cv2

def mask_frame(frame, masks):
    masked_images = []
    for mask in masks:
        masked_images.append(cv2.inRange(frame, mask[0], mask[1]))

    masked_image = masked_images[0].copy()

    for i in range(1, len(masked_images)):
        cv2.add(masked_image, masked_images[i])

    return masked_image

