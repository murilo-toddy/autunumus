#ifndef CONEDETECTION_COLORFILTERING_H
#define CONEDETECTION_COLORFILTERING_H

#include <string>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "../camera/Camera.h"
#include "../config.h"


void apply_hsv_mask_to_matrix();

#endif //CONEDETECTION_COLORFILTERING_H
