#include <iostream>
#include <chrono>

#include "Image.h"
#include "imageProcessing.h"
#include "contourHandler.h"

#define STEP_BY_STEP true


void coneDectectionSampledImages() {
    using std::cout;
    using std::chrono::duration, std::chrono::duration_cast;
    using std::chrono::high_resolution_clock, std::chrono::milliseconds;

    for (int s = 1; s <= SAMPLES; s++) {
        cout << "** Processing sample " << std::to_string(s) << " **\n";
        auto *sampledImage = new Image(s);
        auto processBegin = high_resolution_clock::now();

        // Processes image to find candidate edges
        getBorderedImage(sampledImage);
        auto processedImages = high_resolution_clock::now();

        // Finds all cone contours
        searchContours(sampledImage);
        auto processedContours = high_resolution_clock::now();
        cout << sampledImage->cont.pointingUpContours.size() << " cone(s) found\n";

        // Save images on disk
        sampledImage->saveImagesOnDisk(STEP_BY_STEP);
        auto processEnd = high_resolution_clock::now();

        duration<double, std::milli> processImage = processedImages - processBegin;
        duration<double, std::milli> findContours = processedContours - processedImages;
        duration<double, std::milli> saveOnDisk = processEnd - processedContours;
        double coneDetection = processImage.count() + findContours.count();
        double processTotal = coneDetection + saveOnDisk.count();

        cout << "Processing image: " << processImage.count() << " ms\n";
        cout << "Finding contours: " << findContours.count() << " ms\n";
        cout << "Cone detection:   " << coneDetection << " ms\n";
        cout << "Saving files:     " << saveOnDisk.count() << " ms\n";
        cout << "Iteration time:   " << processTotal << " ms\n";

        cout << "\n";
    }
}


void coneDetectionVideo() {

}
