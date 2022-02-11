#ifndef CONEDETECTION_CONTOURHANDLER_H
#define CONEDETECTION_CONTOURHANDLER_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define AREA_TRESHHOLD 300

vector<vector<Point>> searchContours(const Mat& cannyImage, const bool& showStepByStep);
bool convexContourPointingUp(const vector<Point>& contour);

#endif //CONEDETECTION_CONTOURHANDLER_H
