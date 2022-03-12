#include "imageProcessing.h"

using namespace cv;
using std::vector;

/**
 * @brief Generate a color map to filter for specific colors in hsv spectrum
 * @return map std::map<std::string, std::vector<cv::Scalar>> Color map
 */
std::map<std::string, std::vector<cv::Scalar>> loadColorMap() {
    return {
        { "LOW_RED",  {{ 0, 135, 135 },   { 15, 255, 255 }}  },
        { "HIGH_RED", {{ 159, 135, 135 }, { 179, 255, 255 }} }
    };
}

/**
 * @brief Find edges in an image and add information into image object
 * @param image *Image object with original image loaded
 */
void getBorderedImage(Image *image) {
    cvtColor(image->originalImage, image->mat.originalImageHsv, COLOR_BGR2HSV);
    std::map<std::string, std::vector<cv::Scalar>> colorMap = loadColorMap();

    int index = 0;
    vector<Mat> rangedImages(colorMap.size());
    for (auto const &[key, value] : colorMap) {
        cv::inRange(
            image->mat.originalImageHsv,
            value[0], value[1],
            rangedImages[index]
        );
        index++;
    }
//    vector<Scalar> hsvLowerSpectrum = LOW_RED;
//    inRange(image->mat.originalImageHsv,
//            hsvLowerSpectrum[0], hsvLowerSpectrum[1],
//            image->mat.lowerMask);
//
//    // Higher end of HSV spectrum
//    vector<Scalar> hsvUpperSpectrum = HIGH_RED;
//    inRange(image->mat.originalImageHsv,
//            hsvUpperSpectrum[0], hsvUpperSpectrum[1],
//            image->mat.upperMask);
    Mat maskedImage = rangedImages[0].clone();
    for (int i = 1; i < (int)colorMap.size(); i++) {
        cv::addWeighted(maskedImage, 1, rangedImages[i], 1, 0, maskedImage);
    }
    image->mat.mask = maskedImage;
//    imshow("as", image->);
//    waitKey(0);
    // Combines both ends
//    addWeighted(image->mat.lowerMask, 1, image->mat.upperMask, 1, 0, image->mat.mask);

    // Function to operate in an image
    Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));

    // Blurs image and performs canny edges detection
    GaussianBlur(image->mat.mask, image->mat.blurredImage, Size(3, 3), 3, 0);
    Canny(image->mat.blurredImage, image->mat.cannyImage, CANNY_LOW, CANNY_HIGH);

    // Increases then decreases image thickness for better edge recognition
    dilate(image->mat.cannyImage, image->mat.dilatedImage, kernel);
    erode(image->mat.dilatedImage, image->mat.erodedImage, kernel);

    image->processedImage = image->mat.erodedImage;
}
