#include "borderLineDetection.h"

std::map<std::string, std::vector<cv::Scalar>> loadColorMapRoadMarkings() {
    // Scalars are in (H, S, V) format
    // Each color contains a lower and upper end
    return {
        { "WHITE",  {{0, 0, 207}, {110, 22, 255}}},
        { "YELLOW", {}}
    };
}

void findRoadMarkings() {
    /*
     * TODO
     * Change image format to birds eye view
     * Filter for road markings
     * Delimit them and save their information in a structure
     */

    // Collect all sample files
    std::vector<cv::String> files;
    cv::glob(BORDER_IMAGES_PATH, files, false);
    std::map<std::string, std::vector<cv::Scalar>> colorMap = loadColorMapRoadMarkings();

    for (const auto& file : files) {
        std::cout << "** Processing sample " << std::string(file) << " **\n";
        cv::Mat image = cv::imread(file), hsv;

        // Generate source points to birds eye view
        cv::Point2f sourceVertices[4] = {
                cv::Point(700, 605),
                cv::Point(890, 605),
                cv::Point(1760, 1030),
                cv::Point(20, 1030)
        };

        // Respective mapping points, size of image
        cv::Point2f destinationVertices[4] = {
                cv::Point(0, 0),
                cv::Point(WIDTH, 0),
                cv::Point(WIDTH, HEIGHT),
                cv::Point(0, HEIGHT)
        };

        cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(sourceVertices, destinationVertices);
        cv::Mat birdsEyeView(HEIGHT, WIDTH, CV_8UC3), invertedPerspectiveMatrix;
        cv::invert(perspectiveMatrix, invertedPerspectiveMatrix);

        cv::warpPerspective(image, birdsEyeView, perspectiveMatrix, birdsEyeView.size(),  cv::INTER_LINEAR, cv::BORDER_CONSTANT);

        // Color filtering
        cv::cvtColor(birdsEyeView, hsv, cv::COLOR_BGR2HSV);
        int index = 0;
        std::vector<cv::Mat> rangedImages(colorMap.size());
        // Filter for specific image colors
        for (auto const& [key, value] : colorMap) {
            cv::inRange(hsv, value[0], value[1], rangedImages[index]);
            index++;
        }

        cv::Mat maskedImage = rangedImages[0].clone();
        for (int i = 1; i < (int)colorMap.size(); i++) {
            cv::add(maskedImage, rangedImages[i], maskedImage);
        }

        // Try to improve gaps
        cv::GaussianBlur(maskedImage, maskedImage, cv::Size(9, 9), 0);
        cv::Mat kernel = cv::Mat::ones(15, 15, CV_8U);
        cv::dilate(maskedImage, maskedImage, kernel);
        cv::erode(maskedImage, maskedImage, kernel);

        cv::morphologyEx(maskedImage, maskedImage, cv::MORPH_CLOSE, kernel);

        const int thresholdVal = 150;
        threshold(maskedImage, maskedImage, thresholdVal, 255, cv::THRESH_BINARY);


        cv::imshow("Image", image);
        cv::waitKey(0);
    }
}
