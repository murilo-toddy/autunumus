#include "fileHandler.h"

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
