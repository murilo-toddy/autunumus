#include "Camera.h"


Camera::Camera(const int camera_index, const int width, const int height) {
    cap.open(camera_index);
    frame_size = cv::Size(width, height);

    std::unique_ptr<float[]> intrinsic_matrix_array = file::load_array_from_file(
            MATRICES_PATH, INTRINSIC_MATRIX_FILE_NAME, 9);
    intrinsic_matrix = cv::Matx33f(intrinsic_matrix_array.get());

    std::unique_ptr<float[]> distortion_matrix_array = file::load_array_from_file(
            MATRICES_PATH, DISTORTION_MATRIX_FILE_NAME, 5);
    distortion_matrix = cv::Vec<float, 5>(distortion_matrix_array.get());

    cv::initUndistortRectifyMap(intrinsic_matrix, distortion_matrix, 
            cv::Matx33f::eye(), intrinsic_matrix, frame_size, CV_32FC1,
            map_x, map_y);
}


cv::Mat Camera::update_frame() {
    cap >> input_matrix;
    if(CORRECT_CAMERA_INPUT_DISTORTION) {
        cv::remap(input_matrix, corrected_matrix, map_x, map_y, cv::INTER_LINEAR);
    } else { corrected_matrix = input_matrix; }
    return corrected_matrix;
}


cv::Mat Camera::get_corrected_frame() {
    return corrected_matrix;
}


cv::Mat Camera::get_original_frame() {
    return input_matrix;
}


void Camera::show_frame() {
    cv::imshow("Camera", corrected_matrix);
}


void Camera::save_frame(const std::string path, const std::string file_name) {
    file::save_opencv_matrix_to_file(path, file_name, corrected_matrix);
}

bool Camera::is_open() {
    return cap.isOpened();
}

