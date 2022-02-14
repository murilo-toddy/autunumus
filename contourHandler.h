#ifndef CONEDETECTION_CONTOURHANDLER_H
#define CONEDETECTION_CONTOURHANDLER_H

#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Image.h"

using std::vector;
using namespace cv;

// Minimum area that object must have to be considered cone
#define AREA_THRESHOLD 100

// Maximum amount of points that simplified shape can have
#define POINTS_THRESHOLD 8

// Maximum aspect ratio a contour can have to be considered a cone
#define ASPECT_RATIO_THRESHOLD 0.8

void searchContours(Image *image);
bool convexContourPointingUp(const vector<Point>& contour);

#endif //CONEDETECTION_CONTOURHANDLER_H
