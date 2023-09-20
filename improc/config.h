// Main configuration
#define DEBUG    true
#define LOG_TIME false

#define SAMPLE_CONE_IMAGES_PATH "sample_images/cone/*"
#define SAVE_OUTPUT_IMAGES_TO_FILE     true

// Camera configuration
#define CORRECT_CAMERA_INPUT_DISTORTION false
#define CAMERA_WIDTH  1280
#define CAMERA_HEIGHT 960

#define CORRECTION_MATRICES_PATH    "./camera-calibration"
#define DISTORTION_MATRIX_FILE_NAME "distortion.mat"
#define INTRINSIC_MATRIX_FILE_NAME  "intrinsic.mat"

// Cone configuration
/*
 * Parameters to estimate cone distance
 * Base distance:         75.0 cm
 * Relative pixel height: 421.8 px
 */
#define CONE_HEIGHT_CONSTANT (75.0 * 421.8)

// Minimum area that object must have to be considered cone
#define AREA_THRESHOLD 300

// Maximum amount of points that simplified shape can have
#define POINTS_THRESHOLD 8

// Maximum aspect ratio a contour can have to be considered a cone
#define ASPECT_RATIO_THRESHOLD 0.8

// Calibration configuration
#define SAVE_CALIBRATION_IMAGES true
#define SAVE_CALIBRATION_MATRIX_TO_FILE true

#define CALIBRATION_IMAGE_WIDTH  1280
#define CALIBRATION_IMAGE_HEIGHT 960

#define SAMPLE_CALIBRATION_IMAGES_PATH "./sample_images/calibration"
#define CALIBRATION_IMAGES_OUTPUT_PATH "./camera-calibration/calibration/output_images"
#define CHESSBOARD_HEIGHT  8
#define CHESSBOARD_WIDTH   8

