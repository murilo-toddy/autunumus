#ifndef CONEDETECTION_BORDERLINEDETECTION_H
#define CONEDETECTION_BORDERLINEDETECTION_H

#include <iostream>
#include <vector>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


#define BORDER_IMAGES_PATH "../sample_images/line"
#define BORDER_SAMPLE_IMAGES_PATH "../sample_images/line/*.jpg"

void findRoadMarkings();

#endif //CONEDETECTION_BORDERLINEDETECTION_H
