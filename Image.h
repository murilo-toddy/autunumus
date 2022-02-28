#ifndef CONEDETECTION_IMAGE_H
#define CONEDETECTION_IMAGE_H

#include <string>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "fileHandler.h"

#define SOURCE_FOLDER      "../source/"
#define DESTINATION_FOLDER "../output/"


typedef struct {
    cv::Mat originalImageHsv;
    cv::Mat lowerMask, upperMask, mask;
    cv::Mat blurredImage, cannyImage, dilatedImage, erodedImage;
    cv::Mat defaultContours, approximatedContours, convexContours;
    cv::Mat coneContours;
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
    [[nodiscard]] cv::Mat createFixedSizeMatrix() const;
    [[nodiscard]] std::vector<std::vector<Point>> createFixedSizeVector() const;

    void writeOnDisk(const string& fileName, const Mat& matrix);

public:
    cv::Mat originalImage;
    cv::Mat processedImage;
    cv::Mat finalImage;
    Transformations mat;
    Contours cont;

    explicit Image(const int& identifier);

    void configureContourMatrices();
    void configureContourVectors();

    void saveImagesOnDisk(const bool& saveStepByStep);
    void openImages(const bool& showStepByStep) const;
};


#endif //CONEDETECTION_IMAGE_H
