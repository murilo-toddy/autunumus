#ifndef CONEDETECTION_IMAGE_H
#define CONEDETECTION_IMAGE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>

using namespace cv;

#define SOURCE_FOLDER "../source/"
#define DESTINATION_FOLDER "../output/"


typedef struct {
    Mat originalImageHsv;
    Mat lowerMask, upperMask, mask;
    Mat blurredImage, cannyImage, dilatedImage, erodedImage;
    Mat defaultContours, approximatedContours, convexContours;
    Mat coneContours;
} transformations;


class Image {
private:
    int identifier;
    std::string imagePath;

    void configureDestinationFolder();
    Mat createMatrix();
    Mat createMatrix(const int& rows, const int& cols);

public:
    Mat originalImage;
    Mat finalImage;
    transformations mat;

    explicit Image(const int& identifier);

    void saveImagesOnDisk();
    void openImages();
};


#endif //CONEDETECTION_IMAGE_H
