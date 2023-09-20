#include <string>
#include <vector>
#include <chrono>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "config.h"
#include "shared/include/file-handler/file_handler.hpp"
#include "shared/include/camera/Camera.hpp"
#include "cone-detection/cone_detection.h"


// TODO these functions need to get out of here
int get_time_between_events_in_ms(std::chrono::time_point<std::chrono::system_clock> t1,
        std::chrono::time_point<std::chrono::system_clock> t2) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}


int get_fps(std::chrono::time_point<std::chrono::system_clock> t1,
        std::chrono::time_point<std::chrono::system_clock> t2) {
    return 1000 / get_time_between_events_in_ms(t1, t2);
}


cone_info get_cones_from_image(const cv::Mat& image) {
    auto cones = find_cones(image);

    if (DEBUG) {
        for (const auto &cone: cones.cones) {
            std::cout << "Cone[dist:" << cone.distance <<
                ", left:" << cone.left_boundary <<
                ", right:" << cone.right_boundary << "]" << std::endl;
        }
    }

    if(SAVE_OUTPUT_IMAGES_TO_FILE) {
        // save images
    }
    return cones;
}


void draw_cones_in_image(cv::Mat image, const std::vector<cone_data>& cones) {
    for (auto& cone : cones) {
        cv::Rect rect = cv::boundingRect(cone.contour);
        cv::rectangle(image, rect, DRAWING_COLOR);
        cv::putText(
            image, cone.color + " CONE @ " + std::to_string(cone.distance) + "cm",
            {rect.x, rect.y - 10},
            cv::FONT_HERSHEY_DUPLEX, 0.7, DRAWING_COLOR, 1
        );
    }
}


int main(int argc, char** argv) {
    cv::Mat image;
    IMAGE_INPUT_MODE operation_mode = file::get_input_mode_from_stdin(argc, argv);

    if(operation_mode == SAMPLE_IMAGES_INPUT) {
        auto files = file::load_images_from_query(SAMPLE_CONE_IMAGES_PATH);
        for (const auto & file : files) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = cv::imread(file);

            auto collected_frame = std::chrono::high_resolution_clock::now();
            std::cout << "[INFO] Frame Acquisition took " << 
                get_time_between_events_in_ms(cycle_begin, collected_frame) << "ms" << std::endl;

            auto cones = get_cones_from_image(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            std::cout << "[INFO] Cone Processing took "
                << get_time_between_events_in_ms(collected_frame, processed_cones) << "ms" << std::endl;
            
            cv::imshow("camera-calibration", cones.images.back().second);
            cv::waitKey(0);
        }
    }

    else {
        Camera camera;
        while(camera.is_open()) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = camera.update_frame();
            auto collected_frame = std::chrono::high_resolution_clock::now();

            auto cones = get_cones_from_image(image);
            auto processed_cones = std::chrono::high_resolution_clock::now();

            draw_cones_in_image(image, cones.cones);
            cv::imshow("camera-calibration", image);
            cv::waitKey(1);

            auto cycle_end = std::chrono::high_resolution_clock::now();

            if (LOG_TIME) {
                std::cout << "[INFO] Running at " << get_fps(cycle_begin, cycle_end) << " fps" << std::endl;
                std::cout << "[INFO] Cone Processing took " <<
                          get_time_between_events_in_ms(collected_frame, processed_cones) << "ms" << std::endl;
                std::cout << "[INFO] Frame Acquisition took " <<
                          get_time_between_events_in_ms(cycle_begin, collected_frame) << "ms" << std::endl;
            }
        }
    }
}

