#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include "imageProcessing.h"
#include "contourHandler.h"

using namespace std;
using namespace cv;


int main() {
    string imagePath = "../images/3.jpg";
    Mat image = imread(imagePath);
    Mat canny = getImageCanny(image, false);

    vector<vector<Point>> contours = searchContours(canny, false);

    imshow("Original Image", image);
//    imshow("Canny Image", canny);

    Mat imageWithConeHighlight = image.clone();
    for (int i = 0; i < contours.size(); i++) {
        drawContours(imageWithConeHighlight, contours, i, Scalar(0, 255, 255), 2);
    }

    imshow("Cones Highlighted", imageWithConeHighlight);
    cout << contours.size() << " cones found" << endl;
    waitKey(0);
    return 0;
}
