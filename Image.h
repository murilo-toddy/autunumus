#ifndef CONEDETECTION_IMAGE_H
#define CONEDETECTION_IMAGE_H

#include <string>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "fileHandler.h"

using namespace cv;
using std::to_string;

#define SOURCE_FOLDER      "../source/"
#define DESTINATION_FOLDER "../output/"


typedef struct {
    Mat originalImageHsv;
    Mat lowerMask, upperMask, mask;
    Mat blurredImage, cannyImage, dilatedImage, erodedImage;
    Mat defaultContours, approximatedContours, convexContours;
    Mat coneContours;
} Transformations;

typedef struct {
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    std::vector<std::vector<Point>> filteredContours, convexContours, pointingUpContours;
} Contours;


class Image {
private:
    int identifier;
    std::string imagePath, destinationFolder;

    void configureDestinationFolder() const;
    Mat createFixedSizeMatrix() const;
    std::vector<std::vector<Point>> createFixedSizeVector() const;

    void writeOnDisk(const string& fileName, const Mat& matrix);

public:
    Mat originalImage;
    Mat processedImage;
    Mat finalImage;
    Transformations mat;
    Contours cont;

    explicit Image(const int& identifier);

    void configureContourMatrices();
    void configureContourVectors();

    void saveImagesOnDisk(const bool& saveStepByStep);
    void openImages(const bool& showStepByStep) const;
};


#endif //CONEDETECTION_IMAGE_H
