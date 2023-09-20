#include <vector>
#include <string>
#include <opencv2/core.hpp>


typedef struct {
    std::string color;
    float distance;
    std::vector<cv::Point> contour;
    int left_boundary, right_boundary;
} cone_data;


typedef struct {
    std::vector<cone_data> cones;
    std::vector<std::pair<std::string, cv::Mat>> images;
} cone_info;

