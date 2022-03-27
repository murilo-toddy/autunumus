#ifndef PATHDETECTIONALGORITHM_MAIN_H
#define PATHDETECTIONALGORITHM_MAIN_H

#include "setup/colorFiltering.h"
#include "cone_detection/coneDetection.h"
#include "path_detection/borderLineDetection.h"
#include "setup/cameraCalibration.h"

enum OPERATION_MODE {
    COLOR_PICKING,
    CAMERA_CALIBRATION,
    CONE_DETECTION,
    PATH_DETECTION
};

enum INPUT_MODE {
    SAMPLED_IMAGE,
    CAMERA_INPUT
};

#endif //PATHDETECTIONALGORITHM_MAIN_H
