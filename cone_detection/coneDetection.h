#ifndef PATHDETECTIONALGORITHM_CONEDETECTION_H
#define PATHDETECTIONALGORITHM_CONEDETECTION_H

#include <iostream>
#include <chrono>

#include "../frame/Image.h"
#include "../processing/imageProcessing.h"
#include "../processing/contourHandler.h"
#include "../camera/Camera.h"

#define SOURCE_FOLDER      "../cone_detection/test_images/source/"
#define DESTINATION_FOLDER "../cone_detection/test_images/output/"

#define STEP_BY_STEP   true
#define REAL_TIME_ENV  false

void coneDetectionSampledImages();
[[noreturn]] void coneDetectionVideo();

#endif //PATHDETECTIONALGORITHM_CONEDETECTION_H
