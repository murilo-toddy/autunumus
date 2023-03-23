#ifndef CONEDETECTION_FILEHANDLER_H
#define CONEDETECTION_FILEHANDLER_H

#include <string>
#include <filesystem>

#include <opencv2/imgcodecs.hpp>

namespace file {
    void create_folder(const std::string &path);
    void delete_folder(const std::string &path);
    void configure_folder(const std::string &folder_path);
    void save_opencv_matrix(const std::string &directory, const std::string &file_name, const cv::Mat& matrix);
}

#endif //CONEDETECTION_FILEHANDLER_H
