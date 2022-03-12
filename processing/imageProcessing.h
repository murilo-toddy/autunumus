#ifndef CONEDETECTION_IMAGEPROCESSING_H
#define CONEDETECTION_IMAGEPROCESSING_H

#include <vector>
#include <map>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../frame/Image.h"

// Canny edges threshold
#define CANNY_LOW       80.0
#define CANNY_HIGH      160.0

void getBorderedImage(Image *image);

#endif //CONEDETECTION_IMAGEPROCESSING_H
