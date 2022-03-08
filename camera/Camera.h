#ifndef PATHDETECTIONALGORITHM_CAMERA_H
#define PATHDETECTIONALGORITHM_CAMERA_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Camera {
private:
    cv::VideoCapture cap;
    cv::Mat inputMat;

public:
    Camera(const int& captureIndex);

    cv::Mat getMat();
    void readFrame();
    void showFrame();
    void saveFrame(const std::string& path);
};


#endif //PATHDETECTIONALGORITHM_CAMERA_H
