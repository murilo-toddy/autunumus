import os
import sys
import logging

import cv2
import numpy as np


logging.basicConfig(
    level=logging.INFO, 
    datefmt="%d-%m-%y %H:%M:%S",
    format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger(__name__)

hue_min, hue_max = 0, 179
sat_min, sat_max = 0, 255
val_min, val_max = 0, 255


def create_trackbar(window_name):
    cv2.namedWindow(window_name, cv2.WINDOW_AUTOSIZE)
    cv2.createTrackbar("Hue Min", window_name, hue_min, 179, lambda _: None)
    cv2.createTrackbar("Hue Max", window_name, hue_max, 179, lambda _: None)
    cv2.createTrackbar("Sat Min", window_name, sat_min, 255, lambda _: None)
    cv2.createTrackbar("Sat Max", window_name, sat_max, 255, lambda _: None)
    cv2.createTrackbar("Val Min", window_name, val_min, 255, lambda _: None)
    cv2.createTrackbar("Val Max", window_name, val_max, 255, lambda _: None)


def apply_color_mask(image_path):
    if not image_path:
        camera = cv2.VideoCapture(0)
    else:
        image = cv2.imread(image_path)

    while True:
        if not image_path:
            _, image = camera.read()
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        hue_min = cv2.getTrackbarPos("Hue Min", "Trackbar")
        hue_max = cv2.getTrackbarPos("Hue Max", "Trackbar")
        sat_min = cv2.getTrackbarPos("Sat Min", "Trackbar")
        sat_max = cv2.getTrackbarPos("Sat Max", "Trackbar")
        val_min = cv2.getTrackbarPos("Val Min", "Trackbar")
        val_max = cv2.getTrackbarPos("Val Max", "Trackbar")

        lower = np.array([hue_min, sat_min, val_min])
        upper = np.array([hue_max, sat_max, val_max])
        mask = cv2.inRange(hsv, lower, upper)

        cv2.imshow("Original image", image)
        cv2.imshow("Masked image", mask)
        c = cv2.waitKey(1)
        if c == 27:
            break

    if not image_path:
        camera.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        logger.info("No arguments passed, reading from webcam")
        image_path = None

    else:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        image_path = os.path.join(script_dir, "samples", f"{sys.argv[1]}")
        logger.info(f"Attemtping to read file {image_path}")

    create_trackbar("Trackbar")
    apply_color_mask(image_path)

