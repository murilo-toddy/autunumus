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
        auto processBegin = std::chrono::high_resolution_clock::now();

        camera.readFrame(); // Read camera input
        auto *cameraFrame = new Image(camera.getMat(), "");
        getBorderedImage(cameraFrame); // Process frame
        searchContours(cameraFrame);   // Find cone contours in frame
        cv::imshow("ConeDetection", cameraFrame->finalImage);
        cv::waitKey(1);
        delete cameraFrame;

        auto processEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> process = processEnd - processBegin;
        std::cout << 1000 / (process.count()) << " fps\n";
    }
}
