<h1 align="center">Image Processing and Path Detection</h1>

A scientific research project focused on characteristic extraction and route mapping for
an autonomous vehicle.

## Contents

### Implemented
- Camera calibration
- Color picking in HSV spectrum based on sampled images
- Deterministic cone detection algorithm
  - Using sampled images
  - Using live video
- Road edge detection algorithm
  - Using sampled images
  - Using live video

### Comming Up
- Cone detection implementation using neural networks
- SLAM implementation


## Configuration

Some functionalities can be changed by modifying constants defined in header files. `main.cpp` 
defines witch algorithm will run when program is executed.

### Camera Calibration

This camera calibration algorithm works based on chessboard detection. Add chessboard input
images to `setup/calibration_images` in `jpg` format.

Calibration matrices will be printed to standard output once execution is finished. Matrices
declared in `camera/matrices.h` are the ones used to generate correction maps to live video.

### Color Picking

A simple algorithm that allows for color filtering in HSV color space, based either in live
video or sampled images.

### Cone Detection Algorithm

Sample images must be saved in `cone_detection/test_images/source` in `jpg`format. After execution,
all output matrices will be saved to `cone_detection/test_images/output`, in a folder
with the same name as the sampled image.

In case of live video, some samples can be stored to `cone_detection/video` if the according
variables are set.

### Path Detection

Sample images must be added to `path_detection/test_images` in `jpg`format.

## Dependencies

This project is built using
- `C++17`
- `OpenCV 4`
- `CMake 3.18.4`

## Execution

CMake's configuration files can be created using `cmake -B build`. In the `build` folder,
use `cd build` to go into build folder, `make` to compile and `./main` to run the code.
