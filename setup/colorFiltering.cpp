#include "colorFiltering.h"

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

void createTrackbar() {
    using namespace cv;
    namedWindow("Trackbar", WINDOW_AUTOSIZE);
    createTrackbar("Hue Min", "Trackbar", &hmin, 179);
    createTrackbar("Hue Max", "Trackbar", &hmax, 179);
    createTrackbar("Sat Min", "Trackbar", &smin, 255);
    createTrackbar("Sat Max", "Trackbar", &smax, 255);
    createTrackbar("Val Min", "Trackbar", &vmin, 255);
    createTrackbar("Val Max", "Trackbar", &vmax, 255);
}

void showImage(const std::string &imagePath) {
    using namespace cv;
    createTrackbar();

    Mat image = imread(imagePath);
    Mat hsv, mask;

    while(true) {
        imshow("Image", image);

        cvtColor(image, hsv, COLOR_BGR2HSV);
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(hsv, lower, upper, mask);

        imshow("HSV", hsv);
        imshow("Mask", mask);

        waitKey(1);
    }
}

void findColorSpectrumVideo() {
    createTrackbar();
    cv::VideoCapture cap(0);
    cv::Mat image, hsv, mask;

    while(cap.isOpened()) {
        cap >> image;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
        cv::Scalar lower(hmin, smin, vmin);
        cv::Scalar upper(hmax, smax, vmax);
        
        cv::inRange(hsv, lower, upper, mask);
        
        cv::imshow("Original image", image);
        cv::imshow("Masked image", mask);
        
        cv::waitKey(1);
    }
}