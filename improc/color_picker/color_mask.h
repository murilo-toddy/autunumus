#ifndef CONEDETECTION_COLORFILTERING_H
#define CONEDETECTION_COLORFILTERING_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define USE_LIVE_VIDEO true


// TODO this should be a different file
[[noreturn]] void findColorSpectrumSampleImage(const std::string &imagePath);
void findColorSpectrumVideo();

#endif //CONEDETECTION_COLORFILTERING_H
