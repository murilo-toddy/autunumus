cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(PathDetectionAlgorithm)

add_compile_options(-Wall -Wextra --pedantic -Wno-unknown-pragmas -Wno-switch
        -Wno-unused-but-set-variable -Wno-pointer-arith -Wno-unused-parameter
    )

include_directories($ENV{STAPI_ROOT_PATH}/include/GenICam)
include_directories($ENV{STAPI_ROOT_PATH}/include/StApi)
link_directories($ENV{STAPI_ROOT_PATH}/lib)
link_directories($ENV{STAPI_ROOT_PATH}/lib/GenICam)
find_package(OpenCV REQUIRED)

set(Genicam_Library libGCBase.so libGenApi.so)
set(StApi_TL_Library libStApi_TL.so)
set(StApi_IP_Library libStApi_IP.so)
set(StApi_GUI_Library libStApi_GUI_qt.so)

set(StApi_Library ${StApi_TL_Library})
set(TARGET "main")
set(SOURCES "coneDetection.cpp")
add_executable(${TARGET} ${SOURCES}
        fileHandler.cpp fileHandler.h
        contourHandler.cpp contourHandler.h
        imageProcessing.cpp imageProcessing.h
        Image.cpp Image.h
        borderLineDetection.cpp borderLineDetection.h
        cameraCalibration.cpp cameraCalibration.h
        colorFiltering.cpp colorFiltering.h
        cameraInput.cpp cameraInput.h)
target_link_libraries(${TARGET} ${OpenCV_LIBS} ${Genicam_Library} ${StApi_Library})
set_target_properties(${TARGET}
        PROPERTIES LINK_FLAGS "${Genicam_LinkFlag} ${StApi_LinkFlag}"
    )
