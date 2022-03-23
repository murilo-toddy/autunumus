#ifndef CONEDETECTION_BORDERLINEDETECTION_H
#define CONEDETECTION_BORDERLINEDETECTION_H

#include <iostream>
#include <vector>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


#define BORDER_IMAGES_PATH "../path_detection/test_images/"
#define BORDER_SAMPLE_IMAGES_PATH "../path_detection/test_images/*.jpg"

#define HEIGHT 1280
#define WIDTH  720

void findRoadMarkings();

#endif //CONEDETECTION_BORDERLINEDETECTION_H
