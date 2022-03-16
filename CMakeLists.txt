cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(PathDetectionAlgorithm)

find_package(OpenCV REQUIRED)

set(TARGET "main")
set(SOURCES "main.cpp")
add_executable(${TARGET} ${SOURCES}
        fileHandler.cpp fileHandler.h
        processing/contourHandler.cpp processing/contourHandler.h
        processing/imageProcessing.cpp processing/imageProcessing.h
        frame/Image.cpp frame/Image.h
        path_detection/borderLineDetection.cpp path_detection/borderLineDetection.h
        setup/cameraCalibration.cpp setup/cameraCalibration.h
        setup/colorFiltering.cpp setup/colorFiltering.h
        cone_detection/coneDetection.cpp cone_detection/coneDetection.h
        camera/Camera.cpp camera/Camera.h
        camera/matrices.h cone_detection/Cone.cpp cone_detection/Cone.h)
target_link_libraries(${TARGET} ${OpenCV_LIBS})
