#include "main.h"

#define PATH "../path_detection/test_images/1.jpg"
OPERATION_MODE operation = COLOR_PICKING;
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
            calibrateCamera();
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
