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


int get_time_between_events_in_ms(std::chrono::time_point<std::chrono::system_clock> t1,
        std::chrono::time_point<std::chrono::system_clock> t2) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}


int get_fps(std::chrono::time_point<std::chrono::system_clock> t1,
        std::chrono::time_point<std::chrono::system_clock> t2) {
    return 1000 / get_time_between_events_in_ms(t1, t2);
}


cone_info get_cones_from_image(cv::Mat image) {
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
        auto files = file::load_images_from_query(SAMPLE_CONE_IMAGES_PATH);
        for(int i = 0; i < files.size(); i++) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = cv::imread(files[i]);

            auto collected_frame = std::chrono::high_resolution_clock::now();
            std::cout << "[INFO] Frame Acquisition took " << 
                get_time_between_events_in_ms(cycle_begin, collected_frame) << "ms" << std::endl;

            auto cones = get_cones_from_image(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            std::cout << "[INFO] Cone Processing took "
                << get_time_between_events_in_ms(collected_frame, processed_cones) << "ms" << std::endl;
            
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
            std::cout << "[INFO] Frame Acquisition took " << 
                get_time_between_events_in_ms(cycle_begin, collected_frame) << "ms" << std::endl;

            auto cones = get_cones_from_image(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            std::cout << "[INFO] Cone Processing took " << 
                get_time_between_events_in_ms(collected_frame, processed_cones) << "ms" << std::endl;
            
            cv::imshow("camera", cones.images.back().second);
            cv::waitKey(1);

            auto cycle_end = std::chrono::high_resolution_clock::now();
            std::cout << "[INFO] Running at " << get_fps(cycle_begin, cycle_end) << " fps" << std::endl;
        }
    }
}

