#ifndef CONEDETECTION_FILEHANDLER_H
#define CONEDETECTION_FILEHANDLER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <ostream>

#include <opencv2/imgcodecs.hpp>

namespace file {
    void create_folder(const std::string &path);
    void delete_folder(const std::string &path);
    void configure_folder(const std::string &folder_path);
    void save_opencv_matrix_to_file(const std::string &directory,
            const std::string &file_name, const cv::Mat& matrix);
    void save_array_to_file(const std::string directory, const std::string file_name, 
            const int size, const float array[]);
    std::unique_ptr<float[]> load_array_from_file(const std::string directory, 
            const std::string file_name, const int size);
    std::vector<cv::String> load_images_from_query(const std::string path);
}

#endif //CONEDETECTION_FILEHANDLER_H
