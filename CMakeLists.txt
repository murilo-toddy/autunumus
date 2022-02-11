cmake_minimum_required(VERSION 3.21)
project( ConeDetection )
find_package( OpenCV REQUIRED )

set(CMAKE_CXX_STANDARD 14)

add_executable(ConeDetection coneDetection.cpp contourHandler.cpp contourHandler.h imageProcessing.cpp imageProcessing.h)
target_link_libraries(ConeDetection ${OpenCV_LIBS})