#include "setup/colorFiltering.h"
#include "cone_detection/coneDetection.h"
#include "path_detection/borderLineDetection.h"
// #include "setup/cameraCalibration.h"
#define PATH "../path_detection/test_images/1.jpg"

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

OPERATION_MODE operation = CONE_DETECTION;
INPUT_MODE inputMode = CAMERA_INPUT;

int main(int, char**) {
    switch (operation) {
        case COLOR_PICKING:
            if (inputMode == SAMPLED_IMAGE) {
                findColorSpectrumSampleImage(PATH);
            } else {
                findColorSpectrumVideo();
            }
            break;
        
        case CAMERA_CALIBRATION:
            // calibrateCamera();
            break;

        case CONE_DETECTION:
            if (inputMode == SAMPLED_IMAGE) {
                coneDetectionSampledImages();
            } else {
                coneDetectionVideo();
            }
            break;

        case PATH_DETECTION:
            findRoadMarkings();
            break;
    }
}

