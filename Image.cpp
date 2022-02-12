#include "Image.h"

#include <iostream>

Image::Image(const int& identifier) {
    this->identifier = identifier;
    this->imagePath = SOURCE_FOLDER + std::to_string(identifier) + ".jpg";
    this->originalImage = imread(this->imagePath);
}

Mat Image::createMatrix(const int& rows, const int& cols) {
    return *new Mat(rows, cols, CV_8UC3, {0, 0, 0});
}

Mat Image::createMatrix() {
    return *new Mat(this->originalImage.rows, this->originalImage.cols, CV_8UC3, {0, 0, 0});
}

void Image::configureDestinationFolder() {}

void Image::saveImagesOnDisk() {
    this->configureDestinationFolder();
}

void Image::openImages() {}





