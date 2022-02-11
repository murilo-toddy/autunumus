#ifndef CONEDETECTION_IMAGEPROCESSING_H
#define CONEDETECTION_IMAGEPROCESSING_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat getImageCanny(const Mat& image, const bool& showStepByStep);

#endif //CONEDETECTION_IMAGEPROCESSING_H
