#ifndef CONEDETECTION_IMAGEPROCESSING_H
#define CONEDETECTION_IMAGEPROCESSING_H

#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "frame/Image.h"


// Colors in (H, S, V) format to be considered
#define HSV_LOW_BOTTOM  {0, 135, 135}
#define HSV_LOW_TOP     {15, 255, 255}
#define HSV_UP_BOTTOM   {159, 135, 135}
#define HSV_UP_TOP      {179, 255, 255}

// Canny edges threshold
#define CANNY_LOW       80.0
#define CANNY_HIGH      160.0

void getBorderedImage(Image *image);

#endif //CONEDETECTION_IMAGEPROCESSING_H
