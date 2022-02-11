#include "contourHandler.h"


vector<vector<Point>> searchContours(const Mat& originalImage, const Mat& cannyImage, const bool& showStepByStep) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    int rows = originalImage.rows, cols = originalImage.cols;
    Mat defaultContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat approximatedContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat convexHullContours(rows, cols, CV_8UC3, {0, 0, 0});

    // Finds all contours in image
    findContours(cannyImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> listOfContours(contours.size());
    vector<vector<Point>> convexContours(contours.size());
    vector<vector<Point>> pointingUpContours(contours.size());
    for(int i = 0; i < contours.size(); i++) {
        // Removes initial noise
        double area = contourArea(contours[i]);
        if(area < AREA_TRESHHOLD) { continue; }

        double perimeter = arcLength(contours[i], true);
        approxPolyDP(contours[i], listOfContours[i], 0.02 * perimeter, true);

        // Default contour found in `findContours`
        drawContours(defaultContours, contours, i, Scalar(30, 255, 255));
        // Contour obtained by `aproxPolyDP`
        drawContours(approximatedContours, listOfContours, i, Scalar(30, 255, 255));

        // Make contour convex
        convexHull(listOfContours[i], convexContours[i]);
        drawContours(convexHullContours, convexContours, i, Scalar(255, 0, 255), 2);
        if(!convexContourPointingUp(convexContours[i])) { continue; }

        pointingUpContours.push_back(convexContours[i]);
    }

    imshow("Default contours", defaultContours);
    imshow("Polygon approximation", approximatedContours);
    imshow("Convex contours", convexHullContours);

    return pointingUpContours;
}


bool convexContourPointingUp(vector<Point> contour) {
    return true;
}
