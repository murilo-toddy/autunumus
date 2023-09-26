#ifndef PATHDETECTIONALGORITHM_CAMERA_HPP
#define PATHDETECTIONALGORITHM_CAMERA_HPP

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include "../file-handler/file_handler.hpp"
#include "../../../config.h"


class Camera {
private:
    cv::VideoCapture cap;
    cv::Mat input_matrix, corrected_matrix;
    cv::Mat map_x, map_y;
    cv::Size frame_size;
    cv::Matx33f intrinsic_matrix;
    cv::Vec<float, 5> distortion_matrix;
    void load_correction_matrices();

public:
    explicit Camera(int camera_index = 0, const int width = CAMERA_WIDTH,
            const int height = CAMERA_HEIGHT);

    cv::Mat get_corrected_frame();
    cv::Mat get_original_frame();
    cv::Mat update_frame();
    void show_frame();
    void save_frame(std::string path, std::string file_name);
    bool is_open();
};


#endif //PATHDETECTIONALGORITHM_CAMERA_HPP
