#ifndef CONEDETECTION_CONTOURHANDLER_H
#define CONEDETECTION_CONTOURHANDLER_H

#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../frame/Image.h"
#include "../cone_detection/coneDetection.h"


/*
 * Parameters to estimate cone distance
 * Base distance:         75.0 cm
 * Relative pixel height: 421.8 px
 */
#define CONE_HEIGHT_CONSTANT 75.0 * 421.8

// Minimum area that object must have to be considered cone
#define AREA_THRESHOLD 300

// Maximum amount of points that simplified shape can have
#define POINTS_THRESHOLD 8

// Maximum aspect ratio a contour can have to be considered a cone
#define ASPECT_RATIO_THRESHOLD 0.8

#define DRAWING_COLOR {0, 255, 255}


void searchContours(Image *image);
float convexContourPointingUp(const std::vector<cv::Point>& contour);

#endif //CONEDETECTION_CONTOURHANDLER_H
