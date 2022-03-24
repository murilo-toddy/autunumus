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

int main(int, char**) {
    OPERATION_MODE operation = PATH_DETECTION;
    switch (operation) {
        case COLOR_PICKING:
             findColorSpectrumSampleImage("../path_detection/test_images/1.jpg");
//            findColorSpectrumVideo();
            break;
        
        case CAMERA_CALIBRATION:
            calibrateCamera();
            break;

        case CONE_DETECTION:
            coneDetectionSampledImages();
//            coneDetectionVideo();
            break;

        case PATH_DETECTION:
            findRoadMarkings();
            break;
    }
}