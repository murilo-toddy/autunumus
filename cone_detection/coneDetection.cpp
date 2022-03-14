#include "coneDetection.h"

/**
 * @brief Search for cones in sampled images
 * @param void
 * @return None
 */
void coneDetectionSampledImages() {
    for (int s = 1; s <= SAMPLES; s++) {
        std::cout << "** Processing sample " << std::to_string(s) << " **\n";
        cv::Mat image = cv::imread(SOURCE_FOLDER + std::to_string(s) + ".jpg");
        auto *sampledImage = new Image(image, DESTINATION_FOLDER + std::to_string(s));
        auto processBegin = std::chrono::high_resolution_clock::now();

        // Process image to find candidate edges
        getBorderedImage(sampledImage);
        auto processedImages = std::chrono::high_resolution_clock::now();

        // Find all cone contours
        searchContours(sampledImage);
        auto processedContours = std::chrono::high_resolution_clock::now();
        std::cout << sampledImage->cont.pointingUpContours.size() << " cone(s) found\n";

        // Save images on disk
        sampledImage->saveImagesOnDisk(STEP_BY_STEP);
        auto processEnd = std::chrono::high_resolution_clock::now();

        // Time processing
        std::chrono::duration<double, std::milli> processImage = processedImages - processBegin;
        std::chrono::duration<double, std::milli> findContours = processedContours - processedImages;
        std::chrono::duration<double, std::milli> saveOnDisk = processEnd - processedContours;
        double coneDetection = processImage.count() + findContours.count();
        double processTotal = coneDetection + saveOnDisk.count();

        std::cout << "Processing image: " << processImage.count() << " ms\n";
        std::cout << "Finding contours: " << findContours.count() << " ms\n";
        std::cout << "Cone detection:   " << coneDetection << " ms\n";
        std::cout << "Saving files:     " << saveOnDisk.count() << " ms\n";
        std::cout << "Iteration time:   " << processTotal << " ms\n";
        std::cout << "\n";
    }
}

/**
 * @brief Find cones using default camera input
 * @param void
 * @return None
 */
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
        std::cout << "fps " << 1000 / (process.count()) << "\n";
    }
}
