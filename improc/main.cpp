#include "config.h"
#include "cone_detection/cone_detection.h"
#include "path_detection/roadmark_detection.h"
#include "camera/Camera.h"
#include "io/file_handler.h"
#include <opencv2/highgui.hpp>
#include <chrono>

#define SAMPLE_IMAGES_PATH "images/samples/*"

enum IMAGE_INPUT_MODE {
    SAMPLE_IMAGES,
    CAMERA_INPUT,
};


const IMAGE_INPUT_MODE operation_mode = CAMERA_INPUT;


int main(int, char**) {
    cv::Mat image;

    if(operation_mode == SAMPLE_IMAGES) {
        auto files = file::load_images_from_query(SAMPLE_IMAGES_PATH);
        for(int i = 0; i < files.size(); i++) {

        }
    }

    else {
        Camera camera;
        while(camera.is_open()) {
            auto cycle_begin = std::chrono::high_resolution_clock::now();
            image = camera.update_frame();
            auto cones = find_cones(image);
            
            cv::imshow("camera", cones.images.back().second);
            cv::waitKey(1);

            auto cycle_end = std::chrono::high_resolution_clock::now();
            std::cout << "fps " << 1000 / std::chrono::duration_cast<
                std::chrono::milliseconds>(cycle_end - cycle_begin).count() << "\n";
        }
    }
}

