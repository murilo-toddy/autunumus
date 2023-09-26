#include "../include/color_mask.hpp"


typedef struct {
    int min, max;
} color_range;

color_range hue{0, 179}, sat{0, 255}, val{0, 255};


/**
 * @brief Create a Trackbar to allow HSV parameters changing
 */
void create_trackbar() {
    cv::namedWindow("Trackbar", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Hue Min", "Trackbar", &hue.min, 179);
    cv::createTrackbar("Hue Max", "Trackbar", &hue.max, 179);
    cv::createTrackbar("Sat Min", "Trackbar", &sat.min, 255);
    cv::createTrackbar("Sat Max", "Trackbar", &sat.max, 255);
    cv::createTrackbar("Val Min", "Trackbar", &val.min, 255);
    cv::createTrackbar("Val Max", "Trackbar", &val.max, 255);
}


/**
 * @brief Apply HSV masking to image or live video
 */
void apply_hsv_mask_to_matrix(IMAGE_INPUT_MODE operation_mode) {
    create_trackbar();
    cv::Mat image, hsv, mask;
    Camera camera;
    if(operation_mode == SAMPLE_IMAGES_INPUT) {
        image = cv::imread(SAMPLE_CONE_IMAGES_PATH);
    }

    while(true) {
        if(operation_mode == CAMERA_INPUT) {
            image = camera.update_frame();
        }
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
        cv::Scalar lower(hue.min, sat.min, val.min);
        cv::Scalar upper(hue.max, sat.max, val.max);
        cv::inRange(hsv, lower, upper, mask);
        
        cv::imshow("Original image", image);
        cv::imshow("Masked image", mask);
        cv::waitKey(1);
    }
}


int main(int argc, char** argv) {
    auto operation_mode = file::get_input_mode_from_stdin(argc, argv);
    apply_hsv_mask_to_matrix(operation_mode);
    return 0;
}

