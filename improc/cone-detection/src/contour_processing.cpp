#include "../include/contour_processing.hpp"
#include <ostream>


std::pair<cv::Point, cv::Point> get_left_and_rightmost_points(
    std::vector<cv::Point> points
) {
    cv::Point leftmost_pt_below_center = points.front();
    cv::Point rightmost_pt_below_center = points.front();
    for(auto& p : points) {
        if (p.x < leftmost_pt_below_center.x) {
            leftmost_pt_below_center = p;
        }
        if (p.x > rightmost_pt_below_center.x) {
            rightmost_pt_below_center = p;
        }
    }
    return {leftmost_pt_below_center, rightmost_pt_below_center};
}


void find_cones_in_contours(
    cone_info& cones,
    const std::string& color,
    std::vector<std::vector<cv::Point>> contours
) {
    // TODO move some of this stuff to other functions
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
            if(p.y < height_center) {
                pts_above_center.push_back(p);
            }
            else {
                pts_below_center.push_back(p);
            }
        }

        auto boundaries = get_left_and_rightmost_points(pts_below_center);
        cv::Point left_boundary = boundaries.first;
        cv::Point right_boundary = boundaries.second;

        
        // Check if aspect ratio qualifies as cone
        auto aspect_ratio = (float)bounding_rect.width / (float)bounding_rect.height;
        if (!assert_contour_is_cone(
            aspect_ratio, left_boundary.x, right_boundary.x, pts_above_center
        )) {
            continue;
        }

        cv::Point higher_point = pts_above_center.front();
        for(auto& p : pts_above_center) {
            if(p.y < higher_point.y) { higher_point = p; }
        }

        cone_contours.push_back({higher_point, left_boundary, right_boundary});

        float distance = CONE_HEIGHT_CONSTANT / bounding_rect.height;
        cones.cones.push_back({color, distance, cone_contours.back(),
                left_boundary.x, right_boundary.x});
    }
}


bool assert_contour_is_cone(
    float aspect_ratio,
    int left_boundary,
    int right_boundary,
    std::vector<cv::Point> pts_above_center
) {
    if(aspect_ratio > ASPECT_RATIO_THRESHOLD) {
        return false;
    }

    // checks if all points above center are enclosed by the left and rightmost points below center
    return all_of(pts_above_center.begin(), pts_above_center.end(),
                  [&left_boundary, &right_boundary](const cv::Point &p) -> bool {
                      return !(p.x < left_boundary || p.x > right_boundary);
                  }
    );
}
