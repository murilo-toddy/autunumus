#include <vector>
#include <string>
#include <opencv2/core.hpp>


typedef struct {
    std::string color;
    float distance;
    float angle;
    float width;
    std::vector<cv::Point> contour;
    int left_boundary, right_boundary;
} landmark;


typedef struct {
    std::vector<landmark> cones;
    std::vector<std::pair<std::string, cv::Mat>> images;
} cone_info;

