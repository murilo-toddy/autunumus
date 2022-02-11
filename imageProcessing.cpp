#include "imageProcessing.h"

/**
 * @brief Find canny edges in an image
 * @param image Original image
 * @param showStepByStep Show all intermediate steps
 * @return Image with canny edges
 */
Mat getImageCanny(const Mat& image, const bool& showStepByStep) {
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    // HSV Limit - (hmin, smin, vmin), (hmax, smax, vmax)
    // Lower and of HSV spectrum
    Mat lowerMask;
    vector<Scalar> hsvLowerSpectrum = {HSV_LOW_BOTTOM, HSV_LOW_TOP};
    inRange(hsv, hsvLowerSpectrum[0], hsvLowerSpectrum[1], lowerMask);

    // Higher end of HSV spectrum
    Mat upperMask;
    vector<Scalar> hsvUpperSpectrum = {HSV_UP_BOTTOM, HSV_UP_TOP};
    inRange(hsv, hsvUpperSpectrum[0], hsvUpperSpectrum[1], upperMask);

    // Combines both ends
    Mat mask;
    addWeighted(lowerMask, 1, upperMask, 1, 0, mask);

    // Function to operate in an image
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));

    Mat dilatedImage, erodedImage, blurredImage;
    // Increases then decreases image thickness for better edge recognition
    dilate(mask, dilatedImage, kernel);
    erode(dilatedImage, erodedImage, kernel);
    GaussianBlur(erodedImage, blurredImage, Size(3, 3), 3, 0);

    Mat cannyImage;
    Canny(blurredImage, cannyImage, CANNY_LOW, CANNY_HIGH);

    if (showStepByStep) {
        imshow("HSV", hsv);
        imshow("HSV Low", lowerMask);
        imshow("HSV High", upperMask);
        imshow("Mask", mask);
        imshow("Dilated", dilatedImage);
        imshow("Eroded", erodedImage);
        imshow("Blurred", blurredImage);
        imshow("Canny", cannyImage);
    }

    return cannyImage;
}