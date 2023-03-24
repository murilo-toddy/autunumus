cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(PathDetectionAlgorithm)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") 


set(FILE_HANDLER_SOURCES
    "file_ops/file_handler.cpp"
    "file_ops/file_handler.h"
)

# Main cone and path detection algorithm
set(TARGET "main")
set(SOURCES "main.cpp")
add_executable(${TARGET} ${SOURCES}
    ${FILE_HANDLER_SOURCES}
    processing/imageProcessing.cpp processing/imageProcessing.h
    processing/contourHandler.cpp processing/contourHandler.h
    frame/Image.cpp frame/Image.h
    path_detection/borderLineDetection.cpp path_detection/borderLineDetection.h
    cone_detection/coneDetection.cpp cone_detection/coneDetection.h
    camera/Camera.cpp camera/Camera.h
    camera/matrices.h cone_detection/Cone.cpp
    cone_detection/Cone.h
)

# Camera calibration using Zhang's method
set(CALIBRATE "calibrate")
set(CALIBRATE_SOURCES 
    "camera/calibration/calibrate_camera.cpp"
    "camera/calibration/calibrate_camera.h"
)

add_executable(${CALIBRATE}
    ${CALIBRATE_SOURCES}
    ${FILE_HANDLER_SOURCES}
)


# Color picking using HSV mask
set(COLOR_PICKER "color_picker")
set(COLOR_PICKER_SOURCES 
    "color_picker/color_mask.cpp"
    "color_picker/color_mask.h"
)

add_executable(${COLOR_PICKER}
    ${COLOR_PICKER_SOURCES}
)


target_link_libraries(${TARGET} ${OpenCV_LIBS})
target_link_libraries(${CALIBRATE} ${OpenCV_LIBS})
target_link_libraries(${COLOR_PICKER} ${OpenCV_LIBS})

