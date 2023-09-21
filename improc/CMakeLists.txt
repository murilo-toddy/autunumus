cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(PathDetectionAlgorithm)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
find_package(cppzmq)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") 

set(CONFIG_SOURCE "config.h")

set(FILE_HANDLER_SOURCES
    "shared/src/file-handler/file_handler.cpp"
    "shared/include/file-handler/file_handler.hpp"
)

set(CAMERA_SOURCES
    "shared/src/camera/Camera.cpp"
    "shared/include/camera/Camera.hpp"
)

set(CAMERA_DEPENDENCIES ${CAMERA_SOURCES}
    ${FILE_HANDLER_SOURCES} ${CONFIG_SOURCE}
)


set(CONE_DETECTION_SOURCES
    "cone-detection/src/cone_detection.cpp"
    "cone-detection/include/cone_detection.hpp"
    "cone-detection/include/cone.hpp"
)

set(CONE_DETECTION_DEPENDENCIES 
    ${CONE_DETECTION_SOURCES} ${CAMERA_DEPENDENCIES}
    "cone-detection/src/contour_processing.cpp"
    "cone-detection/include/contour_processing.hpp"
)


# Main cone and path detection algorithm
set(TARGET "main")
set(MAIN "main.cpp" "config.h")
add_executable(${TARGET} ${MAIN}
    ${CONE_DETECTION_DEPENDENCIES}
)

# Camera calibration using Zhang's method
set(CALIBRATE "calibrate")
set(CALIBRATE_SOURCES 
    "camera-calibration/src/camera_calibration.cpp"
    "camera-calibration/include/camera_calibration.hpp"
        cone-detection/src/ZmqProvider.cpp
        cone-detection/include/ZmqProvider.hpp
)

add_executable(${CALIBRATE}
    ${CALIBRATE_SOURCES}
    ${FILE_HANDLER_SOURCES}
)


# Color picking using HSV mask
set(COLOR_PICKER "mask")
set(COLOR_PICKER_SOURCES 
    "color-mask/src/color_mask.cpp"
    "color-mask/include/color_mask.hpp"
)

add_executable(${COLOR_PICKER}
    ${COLOR_PICKER_SOURCES}
    ${CAMERA_DEPENDENCIES}
)


target_link_libraries(${TARGET} PRIVATE ${OpenCV_LIBS} cppzmq)
target_link_libraries(${CALIBRATE} ${OpenCV_LIBS})
target_link_libraries(${COLOR_PICKER} ${OpenCV_LIBS})

