#ifndef CONEDETECTION_IMAGE_H
#define CONEDETECTION_IMAGE_H

#include <string>
#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "../fileHandler.h"


typedef struct {
    cv::Mat originalImageHsv, mask;
    cv::Mat blurredImage, cannyImage, dilatedImage, erodedImage;
    cv::Mat defaultContours, approximatedContours, convexContours;
    cv::Mat coneContours;
} Transformations;

typedef struct {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> filteredContours, convexContours, pointingUpContours;
} Contours;


class Image {
private:
    int identifier;
    std::string imagePath, destinationFolder;

    void configureDestinationFolder() const;
    [[nodiscard]] cv::Mat createFixedSizeMatrix() const;
    [[nodiscard]] std::vector<std::vector<cv::Point>> createFixedSizeVector() const;

    void writeOnDisk(const std::string& fileName, const cv::Mat& matrix);

public:
    cv::Mat originalImage;
    cv::Mat processedImage;
    cv::Mat finalImage;
    Transformations mat;
    Contours cont;

    explicit Image(const std::string& imagePath, const std::string& destinationFolder);

    void configureContourMatrices();
    void configureContourVectors();

    void saveImagesOnDisk(const bool& saveStepByStep);
    void openImages(const bool& showStepByStep) const;
};


#endif //CONEDETECTION_IMAGE_H
