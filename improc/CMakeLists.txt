cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(PathDetectionAlgorithm)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") 

set(CONFIG_SOURCE "config.h")

set(FILE_HANDLER_SOURCES
    "io/file_handler.cpp"
    "io/file_handler.h"
)

set(CAMERA_SOURCES
    "camera/Camera.cpp"
    "camera/Camera.h"
)

set(CAMERA_DEPENDENCIES ${CAMERA_SOURCES} 
    ${FILE_HANDLER_SOURCES} ${CONFIG_SOURCE}
)


set(CONE_DETECTION_SOURCES
    "cone_detection/cone_detection.cpp"
    "cone_detection/cone_detection.h"
)

set(CONE_DETECTION_DEPENDENCIES 
    ${CONE_DETECTION_SOURCES} ${CAMERA_DEPENDENCIES}
    "cone_detection/frame/Image.cpp"
    "cone_detection/frame/Image.h"
    "cone_detection/contour_processing.cpp"
    "cone_detection/contour_processing.h"
)


set(PATH_DETECTION_SOURCES
    "path_detection/roadmark_detection.cpp"
    "path_detection/roadmark_detection.h"
)


# Main cone and path detection algorithm
set(TARGET "main")
set(MAIN "main.cpp" "config.h")
add_executable(${TARGET} ${MAIN}
    ${CONE_DETECTION_DEPENDENCIES}
    ${PATH_DETECTION_SOURCES}
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
set(COLOR_PICKER "mask")
set(COLOR_PICKER_SOURCES 
    "color_picker/color_mask.cpp"
    "color_picker/color_mask.h"
)

add_executable(${COLOR_PICKER}
    ${COLOR_PICKER_SOURCES}
    ${CAMERA_DEPENDENCIES}
)


target_link_libraries(${TARGET} ${OpenCV_LIBS})
target_link_libraries(${CALIBRATE} ${OpenCV_LIBS})
target_link_libraries(${COLOR_PICKER} ${OpenCV_LIBS})

