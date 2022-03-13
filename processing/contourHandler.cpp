#include "contourHandler.h"

/**
 * @brief Finds contours for cones in an image and saves them into Image object
 * @param image *Image object with processed matrices loaded
 * @return None
 */
void searchContours(Image *image) {
    // Configure matrices to store each transformation
    image->configureContourMatrices();

    // Find all contours in image
    cv::findContours(
        image->processedImage,
        image->cont.contours, image->cont.hierarchy,
        cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE
    );

    // Configure filtering contour vectors to have fixed size
    image->configureContourVectors();
    cv::Scalar drawingColor(0, 255, 255);

    for (int i = 0; i < (int)image->cont.contours.size(); i++) {
        // Check if contour's area is greater than threshold
        double area = cv::contourArea(image->cont.contours[i]);
        if (area < AREA_THRESHOLD) { continue; }

        // Approximate contour by a simpler polygon and verifies number of vertices
        double perimeter = cv::arcLength(image->cont.contours[i], true);
        cv::approxPolyDP(image->cont.contours[i], image->cont.filteredContours[i], 0.02 * perimeter, true);
        
        // Check for amount of edges
        if (image->cont.filteredContours[i].size() > POINTS_THRESHOLD || 
                image->cont.filteredContours[i].size() < 3
            ) { continue; }

        // Force contours to be convex
        convexHull(image->cont.filteredContours[i], image->cont.convexContours[i]);

        // Check if polygon is pointing upwards
        float distance = convexContourPointingUp(image->cont.convexContours[i]);
        if (distance == -1) { continue; }

        // Polygon represents a cone
        image->cont.pointingUpContours.push_back(image->cont.convexContours[i]);

        if (!REAL_TIME_ENV) {
            // Draw all contours on matrices
            cv::drawContours(image->mat.defaultContours, image->cont.contours, i, drawingColor);
            cv::drawContours(image->mat.approximatedContours, image->cont.filteredContours, i, drawingColor);
            cv::drawContours(image->mat.convexContours, image->cont.convexContours, i, drawingColor, 2);

            // Save image to cone contours mat
            cv::drawContours(image->mat.coneContours, image->cont.pointingUpContours,
                    image->cont.pointingUpContours.size() - 1, drawingColor, 2
            );

            // Draw bounding rectangle and distance estimation
            cv::Rect boundingRectangle = cv::boundingRect(image->cont.pointingUpContours.back());
            cv::rectangle(image->finalImage, boundingRectangle.tl(), boundingRectangle.br(), drawingColor, 2);
            cv::putText(image->finalImage, "Cone " + std::to_string(distance) + "cm",
                    {boundingRectangle.x, boundingRectangle.y - 10},
                    cv::FONT_HERSHEY_DUPLEX, 0.7, drawingColor, 1
            );
        }

        cv::drawContours(image->finalImage, image->cont.pointingUpContours,
                image->cont.pointingUpContours.size() - 1, drawingColor, 2
        );
    }
}


/**
 * @brief Determines if a contour is pointing upwards
 * @param contour Vector of points representing a contour
 * @return Distance to cone. Returns -1 if no cone is found
 */
float convexContourPointingUp(const std::vector<cv::Point>& contour) {
    cv::Rect boundingRectangle = cv::boundingRect(contour);
    double aspectRatio = (float)boundingRectangle.width / (float)boundingRectangle.height;

    // If element's width is bigger than height, return false
    if (aspectRatio > ASPECT_RATIO_THRESHOLD) { return -1; }

    // Gets y center of contour and separates top points from bottom ones
    int yCenter = boundingRectangle.tl().y + (boundingRectangle.height / 2);
    std::vector<cv::Point> pointsAboveCenter;
    std::vector<cv::Point> pointsBelowCenter;
    for (auto& point : contour) {
        if (point.y < yCenter) {
            pointsAboveCenter.push_back(point);
        } else {
            pointsBelowCenter.push_back(point);
        }
    }

    // Get minimum and maximum x values below center in contour
    int leftmostPointBelowCenterX = pointsBelowCenter.front().x;
    int rightmostPointBelowCenterX = pointsBelowCenter.front().x;
    for (auto& point : pointsBelowCenter) {
        if (point.x < leftmostPointBelowCenterX) { leftmostPointBelowCenterX = point.x; }
        if (point.x > rightmostPointBelowCenterX) { rightmostPointBelowCenterX = point.x; }
    }

    // Determine if all top points are within lower bounds
    if (all_of(pointsAboveCenter.begin(), pointsAboveCenter.end(),
       [&leftmostPointBelowCenterX, &rightmostPointBelowCenterX](cv::Point p) -> bool {
            return !(p.x < leftmostPointBelowCenterX || p.x > rightmostPointBelowCenterX);
    })) {
        // Return distance to cone
        return CONE_HEIGHT_CONSTANT / boundingRectangle.height;
    }
    return -1;
}
