#ifndef CONEDETECTION_CAMERACALIBRATION_H
#define CONEDETECTION_CAMERACALIBRATION_H

#include <iostream>
#include <fstream>
#include <ostream>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../../shared/include/file-handler/file_handler.hpp"
#include "../../config.h"


void calibrate_camera();

#endif //CONEDETECTION_CAMERACALIBRATION_H
