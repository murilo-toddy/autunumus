from time import time
import configparser
import logging

import cv2


logging.basicConfig(
    level=logging.INFO, 
    datefmt="%d-%m-%y %H:%M:%S",
    format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger(__name__)


def read_frames(cap: cv2.VideoCapture):
    if not cap.isOpened():
        logger.error("Could not open camera")
        return

    while True:
        start = time()
        _, frame = cap.read()
        cv2.imshow('Input', frame)

        c = cv2.waitKey(1)
        if c == 27:
            logger.info(f"Escape key pressed, stopping")
            return

        end = time()
        logger.info(f"Frame read took {round(end - start, 4)} seconds")


if __name__ == "__main__":
    config = configparser.ConfigParser()
    config.read("config.ini")

    logger.info("Starting application")

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, config.getint("resolution", "width"))
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, config.getint("resolution", "height"))

    read_frames(cap)

    logger.info("Closing application")
    cap.release()
    cv2.destroyAllWindows()

