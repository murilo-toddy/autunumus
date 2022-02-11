#ifndef CONEDETECTION_FILEHANDLER_H
#define CONEDETECTION_FILEHANDLER_H

#include <opencv2/highgui.hpp>
#include <cstdlib>

#define SAMPLES 13
#define SAVE_IMAGES true

using namespace std;
using namespace cv;

void configureFolder(const string& folderName);
void saveOrShowImage(const string& name, const Mat& image);

#endif //CONEDETECTION_FILEHANDLER_H
