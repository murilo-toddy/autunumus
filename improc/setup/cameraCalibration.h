#ifndef CONEDETECTION_CAMERACALIBRATION_H
#define CONEDETECTION_CAMERACALIBRATION_H

#include <iostream>
#include <fstream>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define SAMPLE_IMAGES_PATH "../setup/calibration_images/*.jpg"
#define CHESSBOARD_HEIGHT  8
#define CHESSBOARD_WIDTH   8

void calibrateCamera();

#endif //CONEDETECTION_CAMERACALIBRATION_H
