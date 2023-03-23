#include "file_handler.h"


void file::create_folder(const std::string &path) {
    std::filesystem::create_directory(path);
}


void file::delete_folder(const std::string &path) {
    std::filesystem::remove_all(path);
}


void file::configure_folder(const std::string &path) {
    file::delete_folder(path);
    file::create_folder(path);
}


void file::save_opencv_matrix(const std::string &directory, const std::string &file_name, const cv::Mat &matrix) {
    cv::imwrite(directory + "/" + file_name, matrix);
}

