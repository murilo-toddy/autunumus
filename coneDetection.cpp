#include <opencv2/imgcodecs.hpp>
#include <iostream>

#include "Image.h"
#include "imageProcessing.h"
#include "contourHandler.h"
#include "fileHandler.h"

using namespace std;
using namespace cv;

#define STEP_BY_STEP true

int main() {
    for (int s = 0; s < SAMPLES; s++) {
        string sample = to_string(s + 1);
        auto *sampledImage = new Image(s + 1);

        string imagePath = "../source/" + sample + ".jpg";
        configureFolder(sample);

        cout << "Processing sample " << sample << endl;

        Mat image = imread(imagePath);
        Mat canny = getBorderedImage(sampledImage, STEP_BY_STEP);
        vector<vector<Point>> contours = searchContours(sample, canny, STEP_BY_STEP);

        saveOrShowImage(sample + "/00original", image);

        Mat imageWithConeHighlight = image.clone();
        for (int i = 0; i < contours.size(); i++) {
            drawContours(imageWithConeHighlight, contours, i, Scalar(0, 255, 255), 2);
        }

        saveOrShowImage(sample + "/final", imageWithConeHighlight);
        cout << contours.size() << " cone(s) found" << endl;
    }
    return 0;
}
