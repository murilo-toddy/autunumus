#include "cone_detection.h"


/**
 * @brief Return a list of masks  for colors of interest
 * Each color contains a list of scalars in 
 * {(Hmin Smin Vmin) (Hmax, Smax, Vmax)} format
 */
const std::vector<std::pair<std::string, std::vector<std::pair<
        cv::Scalar, cv::Scalar>>>> get_color_masks() {
    return {
        {"YELLOW", {
            {{ 16, 188, 116}, { 63, 255, 238}},
        }},
        {"BLUE", {
            {{ 88, 134, 125}, {132, 236, 200}},
        }},
        {"RED", {
            {{  0, 120,  80}, { 15, 255, 255}},
            {{159, 135,  90}, {179, 255, 160}},
        }},
    };
}


cv::Mat mask_image(cv::Mat image, 
        std::vector<std::pair<cv::Scalar, cv::Scalar>> masks) {
    std::vector<cv::Mat> masked_images(masks.size());
    for(int i = 0; i < masks.size(); i++) {
        cv::inRange(image, masks[i].first, masks[i].second, masked_images[i]);
    }
    cv::Mat masked_image = masked_images[0].clone();
    for(int i = 1; i < masks.size(); i++) {
        cv::add(masked_image, masked_images[i], masked_image);
    }
    return masked_image;
}


std::vector<std::vector<cv::Point>> get_contours_from_image(cv::Mat current,
        cone_info &cones, std::string color) {
    auto gaussian_blur_kernel = cv::Size(3, 3);
    cv::Mat gaussian_blur, canny, dilated, eroded;

    cv::GaussianBlur(current, gaussian_blur, gaussian_blur_kernel, 3, 0);
    cones.images.push_back({"4_" + color + "_GAUSSIAN", gaussian_blur});
    current = gaussian_blur;

    cv::Canny(current, canny, CANNY_THRESH_LOW, CANNY_THRESH_HIGH);
    cones.images.push_back({"5_" + color + "_CANNY", canny});
    current = canny;

    auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(current, dilated, kernel);
    cones.images.push_back({"6_" + color + "_DILATED", dilated});
    current = dilated;

    cv::erode(current, eroded, kernel);
    cones.images.push_back({"7_" + color + "_ERODED", eroded});
    current = eroded;

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(current, contours, hierarchy, cv::RETR_EXTERNAL, 
            cv::CHAIN_APPROX_SIMPLE);
    return contours;
}


cone_info find_cones(cv::Mat image) {
    cone_info cones;
    cones.images.push_back({"1_ORIGINAL", image});
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cones.images.push_back({"2_HSV", hsv});

    auto color_masks = get_color_masks();
    std::vector<cv::Mat> masked_images(color_masks.size());

    for(int i = 0; i < color_masks.size(); i++) {
        auto color = color_masks[i].first;
        masked_images[i] = mask_image(hsv, color_masks[i].second);
        cones.images.push_back({"3_" + color + "_MASK", 
                masked_images[i]}); 

        auto contours = get_contours_from_image(
                masked_images[i], cones, color);

        find_cones_in_contours(cones, color, contours);
    }
    return cones;
}

