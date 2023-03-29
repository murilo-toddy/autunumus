#ifndef PATHDETECTIONALGORITHM_CONEDETECTION_H
#define PATHDETECTIONALGORITHM_CONEDETECTION_H

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#include "contour_processing.h"
#include "../camera/Camera.h"

#define CANNY_THRESH_LOW  80.0
#define CANNY_THRESH_HIGH 160.0
#define SAMPLES 14

#define SOURCE_FOLDER      "../cone_detection/test_images/source/"
#define DESTINATION_FOLDER "../cone_detection/test_images/output/"

#define STEP_BY_STEP   true

#define REAL_TIME_ENV  false

#define CONTINUOUS_DESTINATION_FOLDER "cone_detection/output"
#define FRAMES_TO_SAVE                20


typedef struct {
    int i;
} cone_data;


typedef struct {
    std::vector<cone_data> cones;
    std::vector<std::pair<std::string, cv::Mat>> images;
} cone_info;



cone_info find_cones(cv::Mat image);


#endif //PATHDETECTIONALGORITHM_CONEDETECTION_H
