#ifndef PATHDETECTIONALGORITHM_CONE_H
#define PATHDETECTIONALGORITHM_CONE_H

#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


class Cone {
private:
    cv::Point top, left, right;
    cv::Rect boundingRectangle;
    std::vector<cv::Point> contour, coneEstimate;
    float distance;

public:
    Cone(std::vector<cv::Point> coneContour);
    std::vector<cv::Point> getConeContour();
    void drawConeContour();
    void drawConeEstimate();
    void estimatePose();
};


#endif //PATHDETECTIONALGORITHM_CONE_H
