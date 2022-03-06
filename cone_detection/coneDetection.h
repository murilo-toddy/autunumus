#ifndef PATHDETECTIONALGORITHM_CONEDETECTION_H
#define PATHDETECTIONALGORITHM_CONEDETECTION_H

#include <iostream>
#include <chrono>

#include "../frame/Image.h"
#include "../imageProcessing.h"
#include "../contourHandler.h"

#define SOURCE_FOLDER      "../cone_detection/test_images/source/"
#define DESTINATION_FOLDER "../cone_detection/test_images/output/"

#define STEP_BY_STEP true

void coneDetectionSampledImages();
void coneDetectionVideo();

#endif //PATHDETECTIONALGORITHM_CONEDETECTION_H
