#ifndef PATHDETECTIONALGORITHM_CAMERA_H
#define PATHDETECTIONALGORITHM_CAMERA_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include "../file/file_handler.h"
#include "../config.h"


#define WIDTH  1280
#define HEIGHT 960
#define MATRICES_PATH "./camera"
#define DISTORTION_MATRIX_FILE_NAME "distortion.mat"
#define INTRINSIC_MATRIX_FILE_NAME  "intrinsic.mat"


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
    explicit Camera(const int camera_index = 0, const int width = WIDTH, 
            const int heigt = HEIGHT);

    cv::Mat get_corrected_frame();
    cv::Mat get_original_frame();
    cv::Mat update_frame();
    void show_frame();
    void save_frame(const std::string path, const std::string file_name);
    bool is_open();
};


#endif //PATHDETECTIONALGORITHM_CAMERA_H
