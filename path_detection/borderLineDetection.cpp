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

    for (auto const& file : files) {
        std::cout << "** Processing sample " << std::string(file) << " **\n";
        cv::Mat image = cv::imread(file), hsv;

        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
        int index = 0;
        std::vector<cv::Mat> rangedImages(colorMap.size());
        // Filter for specific image colors
        for (auto const& [key, value] : colorMap) {
            cv::inRange(hsv, value[0], value[1], rangedImages[index]);
            index++;
        }

        cv::Mat maskedImage = rangedImages[0].clone();
        for (int i = 1; i < (int)colorMap.size(); i++) {
            cv::addWeighted(maskedImage, 1, rangedImages[i], 1, 0, maskedImage);
        }

        cv::imshow("Image", image);
        cv::waitKey(0);
    }
}
