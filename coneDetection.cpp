#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include "imageProcessing.h"
#include "contourHandler.h"

using namespace std;
using namespace cv;


int main() {
    string imagePath = "../images/1.jpg";
    Mat image = imread(imagePath);
    Mat canny = getImageCanny(image, false);

    vector<vector<Point>> contours = searchContours(image, canny, true);

    imshow("Original Image", image);
    imshow("Canny Image", canny);
    waitKey(0);
    return 0;
}
