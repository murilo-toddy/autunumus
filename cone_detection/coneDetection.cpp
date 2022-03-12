#include "coneDetection.h"

void coneDetectionSampledImages() {
    using std::cout;
    using std::chrono::duration, std::chrono::duration_cast;
    using std::chrono::high_resolution_clock, std::chrono::milliseconds;

    for (int s = 1; s <= SAMPLES; s++) {
        cout << "** Processing sample " << std::to_string(s) << " **\n";
        cv::Mat image = cv::imread(SOURCE_FOLDER + std::to_string(s) + ".jpg");
        auto *sampledImage = new Image(image, DESTINATION_FOLDER + std::to_string(s));
        auto processBegin = high_resolution_clock::now();

        // Process image to find candidate edges
        getBorderedImage(sampledImage);
        auto processedImages = high_resolution_clock::now();

        // Find all cone contours
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
    Camera camera;
    while(true) {
        camera.readFrame();
        auto *cameraFrame = new Image(camera.getMat(), "");
        getBorderedImage(cameraFrame);
        searchContours(cameraFrame);
        cv::imshow("ConeDetection", cameraFrame->finalImage);
        cv::imshow("mask", cameraFrame->mat.mask);
        cv::imshow("border", cameraFrame->mat.cannyImage);
        cv::imshow("contour", cameraFrame->mat.convexContours);
        cv::waitKey(1);

        delete cameraFrame;
    }
}
