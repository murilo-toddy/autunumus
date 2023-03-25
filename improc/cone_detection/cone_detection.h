#ifndef PATHDETECTIONALGORITHM_CONEDETECTION_H
#define PATHDETECTIONALGORITHM_CONEDETECTION_H

#include <iostream>
#include <chrono>

#include "frame/Image.h"
#include "processing/image_processing.h"
#include "processing/contour_handler.h"
#include "../camera/Camera.h"

#define SAMPLES 14

#define SOURCE_FOLDER      "../cone_detection/test_images/source/"
#define DESTINATION_FOLDER "../cone_detection/test_images/output/"

#define STEP_BY_STEP   true
#define REAL_TIME_ENV  false

#define CONTINUOUS_DESTINATION_FOLDER "../cone_detection/video/output/"
#define FRAMES_TO_SAVE                20

void cone_detection_from_sample_images();
[[noreturn]] void coneDetectionVideo();

#endif //PATHDETECTIONALGORITHM_CONEDETECTION_H
