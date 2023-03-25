#include "cone_detection/cone_detection.h"
#include "path_detection/roadmark_detection.h"
#define PATH "../path_detection/test_images/1.jpg"

enum OPERATION_MODE {
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
        case CONE_DETECTION:
            if (inputMode == SAMPLED_IMAGE) {
                cone_detection_from_sample_images();
            } else {
                coneDetectionVideo();
            }
            break;

        case PATH_DETECTION:
            findRoadMarkings();
            break;
    }
}

