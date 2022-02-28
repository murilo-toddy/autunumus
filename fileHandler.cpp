#include "fileHandler.h"

void file::configureFolder(const std::string& folderName) {
    system(("rm -r " + folderName).c_str());
    system(("mkdir " + folderName).c_str());
}
