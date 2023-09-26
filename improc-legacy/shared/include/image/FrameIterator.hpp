#ifndef PATHDETECTIONALGORITHM_FRAMEITERATOR_HPP
#define PATHDETECTIONALGORITHM_FRAMEITERATOR_HPP


#include <vector>
#include <opencv2/core/mat.hpp>

class FrameIterator {
private:
    std::vector<cv::Mat>& images_;
    size_t index_;

public:
    FrameIterator(std::vector<cv::Mat>& images) : images_(images), index_(0) {}

};


#endif //PATHDETECTIONALGORITHM_FRAMEITERATOR_HPP
