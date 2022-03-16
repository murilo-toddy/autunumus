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
        camera/StCamera.cpp camera/StCamera.h
        camera/matrices.h cone_detection/Cone.cpp cone_detection/Cone.h)
target_link_libraries(${TARGET} ${OpenCV_LIBS} ${Genicam_Library} ${StApi_Library})
set_target_properties(${TARGET}
        PROPERTIES LINK_FLAGS "${Genicam_LinkFlag} ${StApi_LinkFlag}"
    )
