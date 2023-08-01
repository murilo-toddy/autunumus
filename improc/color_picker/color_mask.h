#ifndef CONEDETECTION_COLORFILTERING_H
#define CONEDETECTION_COLORFILTERING_H

#include <string>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "../camera/Camera.h"

#define SOURCE_IMAGE_PATH "./sample_images/cone/1.jpg"
#define CAMERA_INDEX 0


void apply_hsv_mask_to_matrix();

#endif //CONEDETECTION_COLORFILTERING_H
