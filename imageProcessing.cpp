#include "imageProcessing.h"

/**
 * @brief Find canny edges in an image
 * @param image Original image
 * @param showStepByStep Show all intermediate steps
 * @return Image with canny edges
 */
Mat getBorderedImage(Image *image, const bool& showStepByStep) {
    cvtColor(img->originalImage, img->mat.originalImageHsv, COLOR_BGR2HSV);

    // TODO tidy by using a hsv delimited vector, and use vector of masks instead
    // Lower end of HSV spectrum
    Mat lowerMask;
    vector<Scalar> hsvLowerSpectrum = {HSV_LOW_BOTTOM, HSV_LOW_TOP};
    inRange(img->mat.originalImageHsv,
            hsvLowerSpectrum[0], hsvLowerSpectrum[1],
            img->mat.lowerMask);

    // Higher end of HSV spectrum
    Mat upperMask;
    vector<Scalar> hsvUpperSpectrum = {HSV_UP_BOTTOM, HSV_UP_TOP};
    inRange(img->mat.originalImageHsv,
            hsvUpperSpectrum[0], hsvUpperSpectrum[1],
            img->mat.upperMask);

    // Combines both ends
    Mat mask;
    addWeighted(img->mat.lowerMask, 1, img->mat.upperMask, 1, 0, img->mat.mask);

    // Function to operate in an image
    Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));

    //Matrices to store transformations
    Mat blurredImage, cannyImage, dilatedImage, erodedImage;

    // Blurs image and performs canny edges detection
    GaussianBlur(img->mat.mask, img->mat.blurredImage, Size(3, 3), 3, 0);
    Canny(img->mat.blurredImage, img->mat.cannyImage, CANNY_LOW, CANNY_HIGH);

    // Increases then decreases image thickness for better edge recognition
    dilate(img->mat.cannyImage, img->mat.dilatedImage, kernel);
    erode(img->mat.dilatedImage, img->mat.erodedImage, kernel);

    if (showStepByStep) {
        saveOrShowImage(sample + "/01hsv", img->mat.originalImageHsv);
        saveOrShowImage(sample + "/02hsv_low", img->mat.lowerMask);
        saveOrShowImage(sample + "/03hsv_high", img->mat.upperMask);
        saveOrShowImage(sample + "/04mask", img->mat.mask);
        saveOrShowImage(sample + "/05blurred", img->mat.blurredImage);
        saveOrShowImage(sample + "/06canny", img->mat.cannyImage);
        saveOrShowImage(sample + "/07dilated", img->mat.dilatedImage);
        saveOrShowImage(sample + "/08eroded", img->mat.erodedImage);
    }
    return img->mat.erodedImage;
}