#ifndef CONEDETECTION_CONTOURHANDLER_H
#define CONEDETECTION_CONTOURHANDLER_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "fileHandler.h"

using namespace std;
using namespace cv;

// Minimum area that object must have to be considered cone
#define AREA_THRESHOLD 200

vector<vector<Point>> searchContours(string sample, const Mat& cannyImage, const bool& showStepByStep);
bool convexContourPointingUp(const vector<Point>& contour);

#endif //CONEDETECTION_CONTOURHANDLER_H
