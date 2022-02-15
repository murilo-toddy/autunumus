cmake_minimum_required(VERSION 3.18)
project(ConeDetection)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(main coneDetection.cpp fileHandler.h contourHandler.cpp contourHandler.h imageProcessing.cpp imageProcessing.h fileHandler.cpp Image.cpp Image.h borderLineDetection.cpp borderLineDetection.h cameraCalibration.cpp cameraCalibration.h)
target_link_libraries(main ${OpenCV_LIBS})