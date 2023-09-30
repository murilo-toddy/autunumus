import logging
import numpy as np
import cv2
import glob

CALIBRATION_IMAGE_WIDTH = 1280
CALIBRATION_IMAGE_HEIGHT = 960

CHESSBOARD_WIDTH = 8
CHESSBOARD_HEIGHT = 8

CALIBRATION_IMAGES_OUTPUT_PATH = "./output"
CORRECTION_MATRICES_PATH = "."

logging.basicConfig(
    level=logging.INFO, 
    datefmt="%d-%m-%y %H:%M:%S",
    format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger(__name__)


def calibrate_camera():
    files = glob.glob("./samples/*.jpg")

    pattern_size = (CHESSBOARD_HEIGHT - 1, CHESSBOARD_WIDTH - 1)
    object_points = []
    image_points = []

    object_position = np.zeros(((CHESSBOARD_WIDTH - 1) * (CHESSBOARD_HEIGHT - 1), 3), np.float32)
    object_position[:, :2] = np.mgrid[1:CHESSBOARD_HEIGHT, 1:CHESSBOARD_WIDTH].T.reshape(-1, 2)

    for index, file in enumerate(files):
        logger.info(f"Analyzing {file}")
        image = cv2.imread(file)
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        pattern_found, corners = cv2.findChessboardCorners(gray, pattern_size)

        if pattern_found:
            criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.1)
            cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
            object_points.append(object_position)
            image_points.append(corners)

        cv2.drawChessboardCorners(image, pattern_size, corners, pattern_found)
        cv2.imwrite(f"{CALIBRATION_IMAGES_OUTPUT_PATH}/{index}.jpg", image)

    intrinsic_matrix = np.eye(3)
    distortion_matrix = np.zeros(5)

    _, intrinsic_matrix, distortion_matrix, _, _ = cv2.calibrateCamera(
            object_points, 
            image_points,
            (CALIBRATION_IMAGE_WIDTH, CALIBRATION_IMAGE_HEIGHT),
            intrinsic_matrix, 
            distortion_matrix
        )

    logger.info(f"distortion_matrix: \n{distortion_matrix}")
    logger.info(f"intrinsic_matrix: \n{intrinsic_matrix}")

    np.savetxt(f"{CORRECTION_MATRICES_PATH}/distortion.txt", distortion_matrix)
    np.savetxt(f"{CORRECTION_MATRICES_PATH}/intrinsic.txt", intrinsic_matrix)
    logger.info("Matrices updated")

    map_x, map_y = cv2.initUndistortRectifyMap(
            intrinsic_matrix, distortion_matrix,
            None, intrinsic_matrix, (CALIBRATION_IMAGE_WIDTH, CALIBRATION_IMAGE_HEIGHT), cv2.CV_32FC1
        )

    for index, file in enumerate(files):
        image = cv2.imread(file)
        image_undistorted = cv2.remap(image, map_x, map_y, interpolation=cv2.INTER_LINEAR)

        image_name = f"{index}_corrected.jpg"
        cv2.imwrite(f"{CALIBRATION_IMAGES_OUTPUT_PATH}/{image_name}", image_undistorted)
        logger.info(f"{image_name} saved")


if __name__ == "__main__":
    calibrate_camera()

