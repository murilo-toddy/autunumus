#ifndef CONEDETECTION_FILEHANDLER_H
#define CONEDETECTION_FILEHANDLER_H

#include <opencv2/highgui.hpp>
#include <cstdlib>

#define SAMPLES 13
#define SAVE_IMAGES true

using namespace std;
using namespace cv;

namespace file {
    void configureFolder(const string &folderName);
}

#endif //CONEDETECTION_FILEHANDLER_H
