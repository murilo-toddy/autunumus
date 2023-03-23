#include "file_handler.h"
#include <opencv2/imgcodecs.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void file::create_folder(const std::string &path) {

}

void file::configure_folder(const std::string &folder_path) {
    system(("rm -r " + folder_path).c_str());
    std::filesystem::create_directory(folder_path);
    system(("mkdir " + folder_path).c_str());
}


void file::save_opencv_matrix(const std::string &directory, const std::string &file_name, const cv::Mat &matrix) {
    cv::imwrite(file_name, matrix);
}
