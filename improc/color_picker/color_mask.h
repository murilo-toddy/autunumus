#ifndef CONEDETECTION_COLORFILTERING_H
#define CONEDETECTION_COLORFILTERING_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define SOURCE_IMAGE_PATH "./images/cone_sample/1.jpg"
#define CAMERA_INDEX 0

enum mode {
    CAMERA_INPUT,
    SOURCE_IMAGE
};

const mode operation_mode = SOURCE_IMAGE;


void apply_hsv_mask_to_matrix();

#endif //CONEDETECTION_COLORFILTERING_H
