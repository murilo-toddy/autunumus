#include <opencv2/highgui.hpp>
#include <chrono>

#include "config.h"
#include "file/file_handler.h"
#include "camera/Camera.h"
#include "cone_detection/cone_detection.h"
#include "path_detection/roadmark_detection.h"

#define SAMPLE_IMAGES_PATH "images/cone_sample/*"

enum IMAGE_INPUT_MODE {
    SAMPLE_IMAGES,
    CAMERA_INPUT,
};


const IMAGE_INPUT_MODE operation_mode = SAMPLE_IMAGES;

int main(int, char**) {
    cv::Mat image;

    if(operation_mode == SAMPLE_IMAGES) {
        auto files = file::load_images_from_query(SAMPLE_IMAGES_PATH);
        for(int i = 0; i < files.size(); i++) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = cv::imread(files[i]);

            auto collected_frame = std::chrono::high_resolution_clock::now();
            std::cout << "updating frame took " << std::chrono::duration_cast<
                std::chrono::milliseconds>(collected_frame
                        - cycle_begin).count() << "\n";

            auto cones = find_cones(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            std::cout << "updating cones took " << std::chrono::duration_cast<
                std::chrono::milliseconds>(processed_cones - collected_frame
                ).count() << "\n";
            
            cv::imshow("camera", cones.images.back().second);
            cv::waitKey(0);

            auto cycle_end = std::chrono::high_resolution_clock::now();
            std::cout << "fps " << 1000 / std::chrono::duration_cast<
                std::chrono::milliseconds>(cycle_end - cycle_begin).count() << "\n";
        }
    }

    else {
        Camera camera;
        while(camera.is_open()) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();

            image = camera.update_frame();

            auto collected_frame = std::chrono::high_resolution_clock::now();
            std::cout << "updating frame took " << std::chrono::duration_cast<
                std::chrono::milliseconds>(collected_frame
                        - cycle_begin).count() << "\n";

            auto cones = find_cones(image);

            auto processed_cones = std::chrono::high_resolution_clock::now();
            std::cout << "updating cones took " << std::chrono::duration_cast<
                std::chrono::milliseconds>(processed_cones - collected_frame
                ).count() << "\n";
            
            cv::imshow("camera", cones.images.back().second);
            cv::waitKey(1);

            auto cycle_end = std::chrono::high_resolution_clock::now();
            std::cout << "fps " << 1000 / std::chrono::duration_cast<
                std::chrono::milliseconds>(cycle_end - cycle_begin).count() << "\n";
        }
    }
}
