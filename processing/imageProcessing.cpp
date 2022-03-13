#include "imageProcessing.h"

/**
 * @brief Generate a color map to filter for specific colors in hsv spectrum
 * @return map std::map<std::string, std::vector<cv::Scalar>> Color map
 */
std::map<std::string, std::vector<cv::Scalar>> loadColorMap() {
    // Scalars are in (H, S, V) format
    // Each color contains a lower and upper end
    return {
        { "LOW_RED",  {{ 0, 120, 120 },   { 15, 255, 255 }}  },
        { "HIGH_RED", {{ 159, 135, 135 }, { 179, 255, 255 }} }
    };
}

/**
 * @brief Find edges in an image and add information into image object
 * @param image *Image object with original image loaded
 */
void getBorderedImage(Image *image) {
    cv::cvtColor(image->originalImage, image->mat.originalImageHsv, cv::COLOR_BGR2HSV);
    std::map<std::string, std::vector<cv::Scalar>> colorMap = loadColorMap();

    int index = 0;
    std::vector<cv::Mat> rangedImages(colorMap.size());
    // Filter for specific image colors
    for (auto const &[key, value] : colorMap) {
        cv::inRange(
            image->mat.originalImageHsv,
            value[0], value[1],
            rangedImages[index]
        );
        index++;
    }

    cv::Mat maskedImage = rangedImages[0].clone();
    for (int i = 1; i < (int)colorMap.size(); i++) {
        cv::addWeighted(maskedImage, 1, rangedImages[i], 1, 0, maskedImage);
    }
    image->mat.mask = maskedImage;

    // Function to operate in an image
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));

    // Blurs image and performs canny edges detection
    cv::GaussianBlur(image->mat.mask, image->mat.blurredImage, cv::Size(3, 3), 3, 0);
    cv::Canny(image->mat.blurredImage, image->mat.cannyImage, CANNY_LOW, CANNY_HIGH);

    // Increases then decreases image thickness for better edge recognition
    cv::dilate(image->mat.cannyImage, image->mat.dilatedImage, kernel);
    cv::erode(image->mat.dilatedImage, image->mat.erodedImage, kernel);

    image->processedImage = image->mat.erodedImage;
}
