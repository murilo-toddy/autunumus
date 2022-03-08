#include "Camera.h"

Camera::Camera(const int& captureIndex) {
    this->cap.open(captureIndex);
}

void Camera::readFrame() {
    this->cap >> this->inputMat;
}

cv::Mat Camera::getMat() {
    return this->inputMat;
}

void Camera::showFrame() {
    cv::imshow("Camera", this->inputMat);
}

void Camera::saveFrame(const std::string &path) {
    cv::imwrite(path, this->inputMat);
}
