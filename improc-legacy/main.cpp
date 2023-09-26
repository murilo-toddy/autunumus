#include <string>
#include <vector>
#include <chrono>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "config.h"
#include "landmark-detection/include/ZmqProvider.hpp"
#include "shared/include/file-handler/file_handler.hpp"
#include "shared/include/camera/Camera.hpp"
#include "landmark-detection/include/detection.hpp"


// TODO these functions need to get out of here
int get_time_between_events_in_ms(std::chrono::time_point<std::chrono::system_clock> t1,
        std::chrono::time_point<std::chrono::system_clock> t2) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}


int get_fps(std::chrono::time_point<std::chrono::system_clock> t1,
        std::chrono::time_point<std::chrono::system_clock> t2) {
    return 1000 / get_time_between_events_in_ms(t1, t2);
}


frame_data process_frame(const cv::Mat& image) {
    auto images_and_landmarks = find_landmarks_in_frame(image);
    auto landmarks = images_and_landmarks.landmarks;

    if (DEBUG) {
        for (const auto &landmark: landmarks) {
            std::cout << "Landmark[" << landmark.color << " dist:" << landmark.distance <<
                       ", width:" << landmark.width <<
                       ", angle:" << landmark.angle <<
                       ", left:" << landmark.left_boundary <<
                       ", right:" << landmark.right_boundary << "]" << std::endl;
        }
    }

    if(SAVE_OUTPUT_IMAGES_TO_FILE) {
        // save images
    }
    return images_and_landmarks;
}


void draw_landmarks_in_image(cv::Mat image, const std::vector<landmark>& landmarks) {
    for (auto& landmark : landmarks) {
        cv::Rect rect = cv::boundingRect(landmark.contour);
        cv::rectangle(image, rect, DRAWING_COLOR);
        cv::putText(
                image, landmark.color + " LANDMARK @ " + std::to_string(landmark.distance) + "cm",
                {rect.x, rect.y - 10},
                cv::FONT_HERSHEY_DUPLEX, 0.7, DRAWING_COLOR, 1
        );
    }
}


int main(int argc, char** argv) {
    // TODO a Image.get_next() would work well for both cases
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

            auto cones = process_frame(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            std::cout << "[INFO] Cone Processing took "
                << get_time_between_events_in_ms(collected_frame, processed_cones) << "ms" << std::endl;

            draw_landmarks_in_image(image, cones.landmarks);
            cv::imshow("sampled-image", image);
            cv::waitKey(0);
        }
    }

    else {
        Camera camera;
        ZmqProvider zmq;
        while(camera.is_open()) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = camera.update_frame();
            auto collected_frame = std::chrono::high_resolution_clock::now();

            auto cones = process_frame(image);
            auto processed_cones = std::chrono::high_resolution_clock::now();

            draw_landmarks_in_image(image, cones.landmarks);
            cv::imshow("live video", image);
            cv::waitKey(1);

            auto cycle_end = std::chrono::high_resolution_clock::now();

            if (LOG_TIME) {
                std::cout << "[INFO] Running at " << get_fps(cycle_begin, cycle_end) << " fps" << std::endl;
                std::cout << "[INFO] Cone Processing took " <<
                          get_time_between_events_in_ms(collected_frame, processed_cones) << "ms" << std::endl;
                std::cout << "[INFO] Frame Acquisition took " <<
                          get_time_between_events_in_ms(cycle_begin, collected_frame) << "ms" << std::endl;
            }

//            if (!cones.landmarks.empty()) {
//                zmq.send_cones(cones.landmarks);
//            }
        }
    }
}

