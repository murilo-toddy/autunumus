#ifndef CONEDETECTION_CAMERACALIBRATION_H
#define CONEDETECTION_CAMERACALIBRATION_H

#include <iostream>
#include <fstream>
#include <ostream>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../../file/file_handler.h"

#define DEBUG       true
#define SAVE_IMAGES true
#define SAVE_MATRIX true

#define IMAGE_WIDTH  1280
#define IMAGE_HEIGHT 960

#define SAMPLE_IMAGES_PATH "./sample_images/calibration"
#define OUTPUT_PATH        "./camera/calibration/output_images"
#define CHESSBOARD_HEIGHT  8
#define CHESSBOARD_WIDTH   8

#define CORRECTION_MATRIX_PATH "./camera"


void calibrate_camera();

#endif //CONEDETECTION_CAMERACALIBRATION_H
