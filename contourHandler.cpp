#include "contourHandler.h"

/**
 * @brief Finds contours for cones in an image
 * @param cannyImage Image with canny edges to detect cones
 * @param showStepByStep Show all intermediate steps
 * @return A vector containing cone contours
 */
vector<vector<Point>> searchContours(const Mat& cannyImage, const bool& showStepByStep) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    int rows = cannyImage.rows, cols = cannyImage.cols;
    Mat defaultContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat approximatedContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat convexHullContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat desiredContours(rows, cols, CV_8UC3, {0, 0, 0});

    // Finds all contours in image
    findContours(cannyImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> listOfContours(contours.size());
    vector<vector<Point>> convexContours(contours.size());
    vector<vector<Point>> pointingUpContours;

    for (int i = 0; i < contours.size(); i++) {
        // Removes initial noise
        double area = contourArea(contours[i]);
        if (area < AREA_TRESHHOLD) { continue; }

        double perimeter = arcLength(contours[i], true);
        approxPolyDP(contours[i], listOfContours[i], 0.02 * perimeter, true);

        // Default contour found in `findContours`
        drawContours(defaultContours, contours, i, Scalar(30, 255, 255));
        // Contour obtained by `aproxPolyDP`
        drawContours(approximatedContours, listOfContours, i, Scalar(30, 255, 255));

        // Make contour convex
        convexHull(listOfContours[i], convexContours[i]);
        drawContours(convexHullContours, convexContours, i, Scalar(255, 0, 255), 2);
        if (!convexContourPointingUp(convexContours[i])) { continue; }

        pointingUpContours.push_back(convexContours[i]);
        drawContours(desiredContours, pointingUpContours, pointingUpContours.size() - 1, Scalar(255, 0, 255), 2);
    }

    if (showStepByStep) {
        imshow("Default contours", defaultContours);
        imshow("Polygon approximation", approximatedContours);
        imshow("Convex contours", convexHullContours);
        imshow("Pointing Upwards", desiredContours);
    }

    return pointingUpContours;
}


/**
 * @brief Determines if a contour is pointing upwards
 * @param contour
 * @return Boolean representing contour orientation
 */
bool convexContourPointingUp(vector<Point> contour) {
    Rect boundingRectangle = boundingRect(contour);
    double aspectRatio = boundingRectangle.width / boundingRectangle.height;

    // If element's width is bigger than height, return false
    if (aspectRatio > 0.8) { return false; }

    double yCenter = boundingRectangle.tl().y + (boundingRectangle.height / 2);
    vector<Point> pointsAboveCenter;
    vector<Point> pointsBelowCenter;
    for (auto& point : contour) {
        if (point.y < yCenter) {
            pointsAboveCenter.push_back(point);
        } else {
            pointsBelowCenter.push_back(point);
        }
    }

    Mat defaultContours(960, 1280, CV_8UC3, {0, 0, 0});
    sort(pointsBelowCenter.begin(), pointsBelowCenter.end(),
    [](Point p1, Point p2) {
            return (p1.x < p2.x);
    });
    int leftmostPointBelowCenterX = pointsBelowCenter.front().x;
    int rightmostPointBelowCenterX = pointsBelowCenter.back().x;

    for (auto& pointAbove : pointsAboveCenter) {
        if (pointAbove.x < leftmostPointBelowCenterX || pointAbove.x > rightmostPointBelowCenterX) {
            return false;
        }
    }
    return true;
}
