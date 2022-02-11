#ifndef CONEDETECTION_FILEHANDLER_H
#define CONEDETECTION_FILEHANDLER_H

#include <opencv2/highgui.hpp>
#include <cstdlib>

#define SAMPLES 13
#define SAVE_IMAGES true

void configureFolder(const string& folderName) {
    if(SAVE_IMAGES) {
        system(("rm -r ../destination/" + folderName).c_str());
        system(("mkdir ../destination/" + folderName).c_str());
    }
}

void saveOrShowImage(const string& name, const Mat& image) {
    if (SAVE_IMAGES) {
        imwrite("../destination/" + name + ".jpg", image);
    } else {
        imshow("Original Image", image);
    }
}

#endif //CONEDETECTION_FILEHANDLER_H
