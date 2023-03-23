cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(PathDetectionAlgorithm)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") 

set(TARGET "main")
set(SOURCES "main.cpp")
add_executable(${TARGET} ${SOURCES}
    file_ops/file_handler.cpp file_ops/file_handler.h
    processing/imageProcessing.cpp processing/imageProcessing.h
    processing/contourHandler.cpp processing/contourHandler.h
    frame/Image.cpp frame/Image.h
    path_detection/borderLineDetection.cpp path_detection/borderLineDetection.h
    setup/colorFiltering.cpp setup/colorFiltering.h
    cone_detection/coneDetection.cpp cone_detection/coneDetection.h
    camera/Camera.cpp camera/Camera.h
    camera/matrices.h cone_detection/Cone.cpp
    cone_detection/Cone.h
)

set(CALIBRATE "calibrate")
set(CALIBRATE_SOURCES "camera/calibration/calibrate_camera.cpp"
    "camera/calibration/calibrate_camera.h")
add_executable(${CALIBRATE}
    ${CALIBRATE_SOURCES}
    file_ops/file_handler.cpp file_ops/file_handler.h
)

target_link_libraries(${TARGET} ${OpenCV_LIBS})
target_link_libraries(${CALIBRATE} ${OpenCV_LIBS})

