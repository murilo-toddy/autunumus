#include "contourHandler.h"

/**
 * @brief Finds contours for cones in an image
 * @param cannyImage Image with canny edges to detect cones
 * @param showStepByStep Show all intermediate steps
 * @return A vector containing cone contours
 */
vector<vector<Point>> searchContours(string sample, const Mat& cannyImage, const bool& showStepByStep) {
    // Create matrices to store different transformations
    int rows = cannyImage.rows, cols = cannyImage.cols;
    Mat defaultContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat approximatedContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat convexHullContours(rows, cols, CV_8UC3, {0, 0, 0});
    Mat desiredContours(rows, cols, CV_8UC3, {0, 0, 0});

    // Vectors to store found contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    // Finds all contours in image
    findContours(cannyImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Vectors to store different contour types
    vector<vector<Point>> listOfContours(contours.size());
    vector<vector<Point>> convexContours(contours.size());
    vector<vector<Point>> pointingUpContours;

    for (int i = 0; i < contours.size(); i++) {
        // Checks if contour's area is greater than threshold
        double area = contourArea(contours[i]);
        if (area < AREA_THRESHOLD) { continue; }

        // Approximates contour by a simpler polygon and verifies number of vertices
        double perimeter = arcLength(contours[i], true);
        approxPolyDP(contours[i], listOfContours[i], 0.02 * perimeter, true);
        if (listOfContours[i].size() > POINTS_THRESHOLD || listOfContours[i].size() < 3) { continue; }

        // Forces contours to be convex
        convexHull(listOfContours[i], convexContours[i]);

        // Draws all contours on matrices
        drawContours(defaultContours, contours, i, Scalar(30, 255, 255));
        drawContours(approximatedContours, listOfContours, i, Scalar(30, 255, 255));
        drawContours(convexHullContours, convexContours, i, Scalar(255, 0, 255), 2);

        // Check if polygon is pointing upwards
        if (!convexContourPointingUp(convexContours[i])) { continue; }
        pointingUpContours.push_back(convexContours[i]);
        drawContours(desiredContours, pointingUpContours, pointingUpContours.size() - 1, Scalar(255, 0, 255), 2);
    }

    if (showStepByStep) {
        saveOrShowImage(sample + "/09default_contours", defaultContours);
        saveOrShowImage(sample + "/10polygon", approximatedContours);
        saveOrShowImage(sample + "/11convex", convexHullContours);
        saveOrShowImage(sample + "/12upwards", desiredContours);
    }

    return pointingUpContours;
}


/**
 * @brief Determines if a contour is pointing upwards
 * @param contour Vector of points representing a contour
 * @return Boolean representing contour orientation
 */
bool convexContourPointingUp(const vector<Point>& contour) {
    Rect boundingRectangle = boundingRect(contour);
    double aspectRatio = (float)boundingRectangle.width / (float)boundingRectangle.height;

    // If element's width is bigger than height, return false
    if (aspectRatio > 0.8) { return false; }

    // Gets y center of contour and separates top points from bottom ones
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

    // Sort vectors by x to find left and rightmost points
    sort(pointsBelowCenter.begin(), pointsBelowCenter.end(),
        [](Point p1, Point p2)-> bool {
            return (p1.x < p2.x);
    });
    int leftmostPointBelowCenterX = pointsBelowCenter.front().x;
    int rightmostPointBelowCenterX = pointsBelowCenter.back().x;

    // Determine if all top points are within lower bounds
    if (all_of(pointsAboveCenter.begin(), pointsAboveCenter.end(),
       [&leftmostPointBelowCenterX, &rightmostPointBelowCenterX](Point p) -> bool {
            return !(p.x < leftmostPointBelowCenterX || p.x > rightmostPointBelowCenterX);
    }))
    return true;
}
