#include <opencv2/highgui.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "color_picker/color_mask.h"
#include "config.h"
#include "file/file_handler.h"
#include "camera/Camera.h"
#include "cone_detection/cone_detection.h"
#include "path_detection/roadmark_detection.h"

#define SAMPLE_IMAGES_PATH "sample_images/cone/*"
#define SAVE_OUTPUT_IMAGES_TO_FILE true


void log_fps(
        std::chrono::time_point<std::chrono::system_clock> begin,
        std::chrono::time_point<std::chrono::system_clock> end
    ) {
    std::cout << "fps " << 1000 /
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() 
        << "ms\n";
}


void log_time(std::string message, std::chrono::time_point<std::chrono::system_clock> ts, 
        std::chrono::time_point<std::chrono::system_clock> tf) {
    std::cout << message << " " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(tf - ts).count() 
        << "ms\n";
}


cone_info process_cycle(cv::Mat image) {
    auto cones = find_cones(image);
    
    // TODO improve logging
    for(auto c : cones.cones) {
        std::cout << "dist " << c.distance << std::endl;
        std::cout << "cont " << c.contour << std::endl;
        std::cout << "left " << c.left_boundary << " right " << c.right_boundary << std::endl;
        std::cout << std::endl;
    }

    if(SAVE_OUTPUT_IMAGES_TO_FILE) {
        // save images
    }
    return cones;
}


int main(int argc, char** argv) {
    cv::Mat image;
    IMAGE_INPUT_MODE operation_mode = file::get_input_mode_from_stdin(argc, argv);

    if(operation_mode == SAMPLE_IMAGES_INPUT) {
        auto files = file::load_images_from_query(SAMPLE_IMAGES_PATH);
        for(int i = 0; i < files.size(); i++) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = cv::imread(files[i]);

            auto collected_frame = std::chrono::high_resolution_clock::now();
            log_time("frame acq", cycle_begin, collected_frame);

            auto cones = process_cycle(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            log_time("cone proc", collected_frame, processed_cones);
            
            cv::imshow("camera", cones.images.back().second);
            cv::waitKey(0);
        }
    }

    else {
        Camera camera;
        while(camera.is_open()) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = camera.update_frame();

            auto collected_frame = std::chrono::high_resolution_clock::now();
            log_time("frame acq", cycle_begin, collected_frame);

            auto cones = process_cycle(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            log_time("cone proc", collected_frame, processed_cones);
            
            cv::imshow("camera", cones.images.back().second);
            cv::waitKey(1);

            auto cycle_end = std::chrono::high_resolution_clock::now();
            log_fps(cycle_begin, cycle_end);
        }
    }
}
