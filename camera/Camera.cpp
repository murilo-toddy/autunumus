#include "Camera.h"
#include <iostream>

Camera::Camera(const int &captureIndex, const int &width, const int &height) {
    this->cap.open(captureIndex);
    this->frameSize = cv::Size(width, height);

    this->intrinsicMatrix = cv::Matx33f(INTRINSIC_MATRIX_VALUES);
    this->distortionMatrix = cv::Vec<float, 5>(DISTORTION_MATRIX_VALUES);

    cv::initUndistortRectifyMap(
            this->intrinsicMatrix, this->distortionMatrix, cv::Matx33f::eye(),
            this->intrinsicMatrix, this->frameSize, CV_32FC1,
            this->mapX, this->mapY
    );
}

void Camera::readFrame() {
    this->cap >> this->inputMat;
    cv::remap(this->inputMat, this->correctedMat, this->mapX, this->mapY, cv::INTER_LINEAR);
}

cv::Mat Camera::getMat() {
    return this->correctedMat;
}

void Camera::showFrame() {
    cv::imshow("Camera", this->correctedMat);
}

void Camera::saveFrame(const std::string &path) {
    cv::imwrite(path, this->correctedMat);
}
