#ifndef PATHDETECTIONALGORITHM_CONEDETECTION_H
#define PATHDETECTIONALGORITHM_CONEDETECTION_H

#include <iostream>

#include "contour_processing.h"
#include "../shared/include/camera/Camera.hpp"

#define CANNY_THRESH_LOW  80.0
#define CANNY_THRESH_HIGH 160.0


cone_info find_cones(const cv::Mat& image);


#endif //PATHDETECTIONALGORITHM_CONEDETECTION_H
