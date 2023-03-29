#include "contour_processing.h"
#include <iostream>
#include <ostream>

/**
 * @brief Draw contours on image object
 * @param image Image object
 * @param index Index of the contour
 * @param distance Distance of found cone
 */
cv::Mat drawContours(cv::Mat image, std::vector<std::vector<cv::Point>> contours,
        const float& distance) {
    // Draw bounding rectangle and distance estimation
//    cv::circle(final_image, leftmost_pt_below_center, 2, {255, 0, 0}, 2);
//    cv::circle(final_image, rightmost_pt_below_center, 2, {255, 0, 0}, 2);
//    cv::circle(final_image, higher_point, 2, {255, 0, 0}, 2);
//    cv::line(final_image, leftmost_pt_below_center, higher_point, DRAWING_COLOR, 2);
//    cv::line(final_image, leftmost_pt_below_center, rightmost_pt_below_center, DRAWING_COLOR, 2);
//    cv::line(final_image, rightmost_pt_below_center, higher_point, DRAWING_COLOR, 2);
    cv::Rect boundingRectangle = cv::boundingRect(contours.back());
    cv::rectangle(image, boundingRectangle, DRAWING_COLOR);
    cv::putText(image, "Cone " + std::to_string(distance) + "cm",
                {boundingRectangle.x, boundingRectangle.y - 10},
                cv::FONT_HERSHEY_DUPLEX, 0.7, DRAWING_COLOR, 1
        );
    return image;
}


std::pair<cv::Point, cv::Point> get_left_and_rightmost_points(
        std::vector<cv::Point> points) {
    cv::Point leftmost_pt_below_center = points.front();
    cv::Point rightmost_pt_below_center = points.front();
    for(auto& p : points) {
        if(p.x < leftmost_pt_below_center.x) { leftmost_pt_below_center = p; }
        if(p.x > rightmost_pt_below_center.x) { rightmost_pt_below_center = p; }
    }
    return {leftmost_pt_below_center, rightmost_pt_below_center};
}


cv::Mat find_cones_in_contours(cv::Mat original_image, 
        std::vector<std::vector<cv::Point>> contours) {
    
    cv::Mat final_image = original_image.clone();
    std::vector<std::vector<cv::Point>> filtered_contours(contours.size()),
        convex_contours(contours.size()), contours_pointing_up, cone_contours;

    for(int i = 0; i < contours.size(); i++) {
        if(cv::contourArea(contours[i]) < AREA_THRESHOLD) { continue; }

        double perimeter = cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], filtered_contours[i], 0.02 * perimeter, true);
        if (filtered_contours[i].size() > POINTS_THRESHOLD || 
                filtered_contours[i].size() < 3) { 
            continue; 
        }

        convexHull(filtered_contours[i], convex_contours[i]);

        std::vector<cv::Point> contour = convex_contours[i];
        cv::Rect bounding_rect = cv::boundingRect(contour);


        int height_center = bounding_rect.tl().y + (bounding_rect.height / 2);
        std::vector<cv::Point> pts_above_center;
        std::vector<cv::Point> pts_below_center;
        
        for(auto& p : contour) {
            if(p.y < height_center) { pts_above_center.push_back(p); } 
            else { pts_below_center.push_back(p); }
        }

        auto boundaries = get_left_and_rightmost_points(pts_below_center);
        cv::Point left_boundary = boundaries.first;
        cv::Point right_boundary = boundaries.second;

        
        // Check if aspect ratio qualifies as cone
        auto aspect_ratio = (float)bounding_rect.width / (float)bounding_rect.height;
        if(!assert_contour_is_cone(aspect_ratio, left_boundary.x, right_boundary.x, 
                pts_above_center)) {
            continue;
        }

        cv::Point higher_point = pts_above_center.front();
        for(auto& p : pts_above_center) {
            if(p.y < higher_point.y) { higher_point = p; }
        }

        cone_contours.push_back({higher_point, left_boundary, right_boundary});

        float distance = CONE_HEIGHT_CONSTANT / bounding_rect.height;
        final_image = drawContours(original_image, cone_contours, distance);
    }
    return final_image;
}


bool assert_contour_is_cone(float aspect_ratio, int left_boundary,
        int right_boundary, std::vector<cv::Point> pts_above_center) {
    if(aspect_ratio > ASPECT_RATIO_THRESHOLD) { return false; }

    if(!all_of(pts_above_center.begin(), pts_above_center.end(),
        [&left_boundary, &right_boundary](const cv::Point& p) -> bool {
                return !(p.x < left_boundary || p.x > right_boundary);
        })) {
        return false;
    }
    return true;
}

