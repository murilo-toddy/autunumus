#include "setup/colorFiltering.h"
#include "cone_detection/coneDetection.h"
#include "setup/cameraCalibration.h"

enum OPERATION_MODE {
    COLOR_PICKING,
    CAMERA_CALIBRATION,
    CONE_DETECTION,
    PATH_DETECTION
};

int main(int, char**) {
    OPERATION_MODE operation = COLOR_PICKING;
    switch (operation) {
        case COLOR_PICKING:
            bool TEST_IN_FIXED_SAMPLE = true;
            if (TEST_IN_FIXED_SAMPLE) {
                findColorSpectrumSampleImage(
                    "../cone_detection/test_images/source/14.jpg"
                );
            } else {
                findColorSpectrumVideo();
            }
            break;
        
        case CAMERA_CALIBRATION:
            calibrateCamera();
            break;

        case CONE_DETECTION:
            coneDetectionVideo();
            break;

        case PATH_DETECTION:
            break;
    }
}