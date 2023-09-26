import logging
import cv2
import numpy as np


class WebcamProvider:
    def __init__(self, width: int, height: int, capture_channel: int = 0):
        self.logger = logging.getLogger(__name__)
        self.width = width
        self.height = height

        self.cap = cv2.VideoCapture(capture_channel)
        self.__setup()

    def __setup(self):
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, self.width)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, self.height)
        if not self.cap.isOpened():
            self.logger.error("Could not open capture channel")
            raise Exception("Could not open capture channel")

    def get_frame(self) -> np.ndarray:
        _, frame = self.cap.read()
        return frame

    def stop(self):
        self.cap.release()

