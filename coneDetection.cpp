#include <opencv2/imgcodecs.hpp>
#include <iostream>

#include "Image.h"
#include "imageProcessing.h"
#include "contourHandler.h"
#include "fileHandler.h"

using namespace std;
using namespace cv;

#define STEP_BY_STEP true

int main() {
    for (int s = 1; s <= SAMPLES; s++) {
        cout << "Processing sample " << to_string(s) << endl;
        auto *sampledImage = new Image(s);

        // Processes image to find candidate edges
        getBorderedImage(sampledImage);

        // Finds all cone contours
        searchContours(sampledImage);

        sampledImage->saveImagesOnDisk(STEP_BY_STEP);
    }
    return 0;
}
