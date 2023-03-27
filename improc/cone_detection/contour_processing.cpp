#include "contour_processing.h"


/**
 * @brief Draw contours on image object
 * @param image Image object
 * @param index Index of the contour
 * @param distance Distance of found cone
 */
cv::Mat drawContours(cv::Mat image, std::vector<std::vector<cv::Point>> contours,
        const float& distance) {
    // Draw bounding rectangle and distance estimation
    cv::Rect boundingRectangle = cv::boundingRect(contours.back());
    cv::rectangle(image, boundingRectangle, DRAWING_COLOR);
    cv::putText(image, "Cone " + std::to_string(distance) + "cm",
                {boundingRectangle.x, boundingRectangle.y - 10},
                cv::FONT_HERSHEY_DUPLEX, 0.7, DRAWING_COLOR, 1
        );
    return image;
}


/**
 * @brief Finds contours for cones in an image and saves them into Image object
 * @param image *Image object with processed matrices loaded
 * @return None
 */
cv::Mat find_cones_in_contours(cv::Mat original_image, std::vector<std::vector<cv::Point>> contours) {

    auto final_image = original_image.clone();

    // Configure filtering contour vectors to have fixed size
    cv::Scalar drawingColor(0, 255, 255);
    std::vector<std::vector<cv::Point>> filtered_contours(contours.size()),
        convex_contours(contours.size()), contours_pointing_up;

    for(int i = 0; i < contours.size(); i++) {
        // Check if contour's area is greater than threshold
        double area = cv::contourArea(contours[i]);
        if(area < AREA_THRESHOLD) { continue; }

        // Approximate contour by a simpler polygon and verifies number of vertices
        double perimeter = cv::arcLength(contours[i], true);

        cv::approxPolyDP(contours[i], filtered_contours[i], 0.02 * perimeter, true);
        
        // Check for amount of edges
        if (filtered_contours[i].size() > POINTS_THRESHOLD || 
                filtered_contours[i].size() < 3) { 
            continue; 
        }

        // Force contours to be convex
        convexHull(filtered_contours[i], convex_contours[i]);

        // Check if polygon is pointing upwards
        float distance = convexContourPointingUp(convex_contours[i]);
        if(distance == -1) { continue; }

        // Polygon represents a cone
        contours_pointing_up.push_back(convex_contours[i]);

        // TODO optimize
        std::vector<cv::Point> contour = contours_pointing_up.back();
        cv::Rect bounding_rect = cv::boundingRect(contour);
        int yCenter = bounding_rect.tl().y + bounding_rect.height / 2;

        std::vector<cv::Point> pts_above_center;
        std::vector<cv::Point> pts_below_center;
        for(auto& point : contour) {
            if(point.y < yCenter) {
                pts_above_center.push_back(point);
            } else {
                pts_below_center.push_back(point);
            }
        }

        // Get minimum and maximum x values below center in contour
        cv::Point leftmostPointBelowCenter = pts_below_center.front();
        cv::Point rightmostPointBelowCenter = pts_below_center.front();
        for(auto& point : pts_below_center) {
            if(point.x < leftmostPointBelowCenter.x) { leftmostPointBelowCenter = point; }
            if(point.x > rightmostPointBelowCenter.x) { rightmostPointBelowCenter = point; }
        }

        cv::Point higherPoint = pts_above_center.front();
        for(auto& point : pts_above_center) {
            if(point.y < higherPoint.y) { higherPoint = point; }
        }

        
        cv::circle(final_image, leftmostPointBelowCenter, 2, {255, 0, 0}, 2);
        cv::circle(final_image, rightmostPointBelowCenter, 2, {255, 0, 0}, 2);
        cv::circle(final_image, higherPoint, 2, {255, 0, 0}, 2);
        cv::line(final_image, leftmostPointBelowCenter, higherPoint, DRAWING_COLOR, 2);
        cv::line(final_image, leftmostPointBelowCenter, rightmostPointBelowCenter, DRAWING_COLOR, 2);
        cv::line(final_image, rightmostPointBelowCenter, higherPoint, DRAWING_COLOR, 2);
        final_image = drawContours(final_image, contours, distance);
    }
    return final_image;
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
    if(aspectRatio > ASPECT_RATIO_THRESHOLD) { return -1; }

    // Gets y center of contour and separates top points from bottom ones
    int yCenter = boundingRectangle.tl().y + (boundingRectangle.height / 2);
    std::vector<cv::Point> pointsAboveCenter;
    std::vector<cv::Point> pointsBelowCenter;
    for(auto& point : contour) {
        if(point.y < yCenter) {
            pointsAboveCenter.push_back(point);
        } else {
            pointsBelowCenter.push_back(point);
        }
    }

    // Get minimum and maximum x values below center in contour
    int leftmostPointBelowCenterX = pointsBelowCenter.front().x;
    int rightmostPointBelowCenterX = pointsBelowCenter.front().x;
    for(auto& point : pointsBelowCenter) {
        if(point.x < leftmostPointBelowCenterX) { leftmostPointBelowCenterX = point.x; }
        if(point.x > rightmostPointBelowCenterX) { rightmostPointBelowCenterX = point.x; }
    }

    // Determine if all top points are within lower bounds
    if(all_of(pointsAboveCenter.begin(), pointsAboveCenter.end(),
       [&leftmostPointBelowCenterX, &rightmostPointBelowCenterX](const cv::Point& p) -> bool {
            return !(p.x < leftmostPointBelowCenterX || p.x > rightmostPointBelowCenterX);
    })) {
        // Return distance to cone
        return CONE_HEIGHT_CONSTANT / boundingRectangle.height;
    }
    return -1;
}

