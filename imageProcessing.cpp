#include "imageProcessing.h"

/**
 * @brief Find canny edges in an image
 * @param image Original image
 * @param showStepByStep Show all intermediate steps
 * @return Image with canny edges
 */
void getBorderedImage(Image *image, const bool& showStepByStep) {
    cvtColor(image->originalImage, image->mat.originalImageHsv, COLOR_BGR2HSV);

    // TODO tidy by using a hsv delimited vector, and use vector of masks instead
    // Lower end of HSV spectrum
    vector<Scalar> hsvLowerSpectrum = {HSV_LOW_BOTTOM, HSV_LOW_TOP};
    inRange(image->mat.originalImageHsv,
            hsvLowerSpectrum[0], hsvLowerSpectrum[1],
            image->mat.lowerMask);

    // Higher end of HSV spectrum
    vector<Scalar> hsvUpperSpectrum = {HSV_UP_BOTTOM, HSV_UP_TOP};
    inRange(image->mat.originalImageHsv,
            hsvUpperSpectrum[0], hsvUpperSpectrum[1],
            image->mat.upperMask);

    // Combines both ends
    addWeighted(image->mat.lowerMask, 1, image->mat.upperMask, 1, 0, image->mat.mask);

    // Function to operate in an image
    Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));

    // Blurs image and performs canny edges detection
    GaussianBlur(image->mat.mask, image->mat.blurredImage, Size(3, 3), 3, 0);
    Canny(image->mat.blurredImage, image->mat.cannyImage, CANNY_LOW, CANNY_HIGH);

    // Increases then decreases image thickness for better edge recognition
    dilate(image->mat.cannyImage, image->mat.dilatedImage, kernel);
    erode(image->mat.dilatedImage, image->mat.erodedImage, kernel);
    
//    if (showStepByStep) {
//        saveOrShowImage(sample + "/01hsv", image->mat.originalImageHsv);
//        saveOrShowImage(sample + "/02hsv_low", image->mat.lowerMask);
//        saveOrShowImage(sample + "/03hsv_high", image->mat.upperMask);
//        saveOrShowImage(sample + "/04mask", image->mat.mask);
//        saveOrShowImage(sample + "/05blurred", image->mat.blurredImage);
//        saveOrShowImage(sample + "/06canny", image->mat.cannyImage);
//        saveOrShowImage(sample + "/07dilated", image->mat.dilatedImage);
//        saveOrShowImage(sample + "/08eroded", image->mat.erodedImage);
//    }
    image->processedImage = image->mat.erodedImage;
}
