#ifndef PATHDETECTIONALGORITHM_CAMERA_H
#define PATHDETECTIONALGORITHM_CAMERA_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include "matrices.h"

class Camera {
private:
    cv::VideoCapture cap;
    cv::Mat inputMat, correctedMat;
    cv::Mat mapX, mapY;
    cv::Size frameSize;
    cv::Matx33f intrinsicMatrix;
    cv::Vec<float, 5> distortionMatrix;

public:
    explicit Camera(
        const int& captureIndex = 0,
        const int& width        = 1280,
        const int& height       = 960
    );

    cv::Mat getMat();
    void readFrame();
    void showFrame();
    void saveFrame(const std::string& path);
};


#endif //PATHDETECTIONALGORITHM_CAMERA_H
