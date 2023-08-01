#include <iostream>
#include "file_handler.h"


void file::create_folder(const std::string &path) {
    std::filesystem::create_directory(path);
}


void file::delete_folder(const std::string &path) {
    if(std::filesystem::exists(std::filesystem::path(path))) {
        std::filesystem::remove_all(path);
    }
}


void file::configure_folder(const std::string &path) {
    file::delete_folder(path);
    file::create_folder(path);
}


void file::save_opencv_matrix_to_file(const std::string &directory, const std::string &file_name, const cv::Mat &matrix) {
    cv::imwrite(directory + "/" + file_name, matrix);
}


void file::save_array_to_file(const std::string directory, 
        const std::string file_name, const int size, const float array[]) {
    std::ofstream file;
    file.open(directory + "/" + file_name);
    for(int i = 0; i < size; i++) { file << array[i] << " "; }
    file.close();
}


std::unique_ptr<float[]> file::load_array_from_file(const std::string directory, 
        const std::string file_name, const int size) {
    auto data = std::make_unique<float[]>(size);
    std::ifstream file;
    file.open(directory + "/" + file_name);
    for(int i = 0; i < size; i++) { file >> data[i]; }
    file.close();
    return data;
}


std::vector<cv::String> file::load_images_from_query(const std::string path) {
    std::vector<cv::String> files;
    cv::glob(path, files, false);
    return files;
}


IMAGE_INPUT_MODE file::get_input_mode_from_stdin(int argc, char **argv) {
     if(argc == 1) {
         return CAMERA_INPUT;
     }
     std::vector<std::string> args;
     std::cout << argc << std::endl;
     args.assign(argv + 1, argv + argc);
     if(args[0] == SAMPLE_IMAGE_STRING) {
         return SAMPLE_IMAGES_INPUT;
     }
    return CAMERA_INPUT;
}

