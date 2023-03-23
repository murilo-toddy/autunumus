#ifndef CONEDETECTION_CAMERACALIBRATION_H
#define CONEDETECTION_CAMERACALIBRATION_H

#include <iostream>
#include <fstream>
#include <ostream>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../../file_ops/file_handler.h"

#define DEBUG       true
#define SAVE_IMAGES true
#define SAVE_MATRIX true

#define IMAGE_WIDTH  1280
#define IMAGE_HEIGHT 960

#define SAMPLE_IMAGES_PATH "./camera/calibration/calibration_source/*"
#define OUTPUT_PATH        "./camera/calibration/calibration_output"
#define CHESSBOARD_HEIGHT  8
#define CHESSBOARD_WIDTH   8

#define CORRECTION_MATRIX_PATH "./camera/correction_matrices"


void calibrate_camera();

#endif //CONEDETECTION_CAMERACALIBRATION_H
