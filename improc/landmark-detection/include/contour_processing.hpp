#ifndef CONEDETECTION_CONTOURHANDLER_H
#define CONEDETECTION_CONTOURHANDLER_H

#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "cone.hpp"
#include "../../config.h"

#define DRAWING_COLOR {0, 255, 255}


void find_cones_in_contours(cone_info& cones, const std::string& color,
        std::vector<std::vector<cv::Point>> contours);
bool assert_contour_is_cone(float aspect_ratio, int left_boundary,
        int right_boundary, std::vector<cv::Point> pts_above_center);

#endif //CONEDETECTION_CONTOURHANDLER_H
