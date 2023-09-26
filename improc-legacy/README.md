# Image Processing

The image processing module is focused mainly on creating a deterministic algorithm to identify
multicolor landmarks in a custom scenario.

## Contents

- Camera calibration
- Color picking in HSV spectrum based on sampled images or input video
- Deterministic cone detection algorithm
    - Using sampled images
    - Using live video

## Configuration

All constants are defined in the `constants.h` file and should be modified to properly
represent the usage case.

### Camera Calibration

This camera calibration algorithm works based on chessboard detection. Add chessboard input
images to `sample_images/calibration` in `jpg` format.

After execution, the new correction matrices will be saved in a file and are ready to be used
on the next main execution.

### Color Picking

A simple algorithm that allows for color filtering in HSV color space, based either in live
video or sampled images.

This allows for the user to easily determine witch colors will be used in the filtering
step of cone detection

### Cone Detection Algorithm

Sample images must be saved in `sample_images/cone` in `jpg`format. After execution,
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

A `Makefile` is present to easily allow for compiling and running the different binaries.

- `make all` sets up the build folder with the `cmake` definitions
- `make compile` compiles all binaries
- `make run` executes the cone detection program using camera as input
- `make run_sample` executes the cone detection program passing in sample images
- `make run_calibration` executes the calibration steps for a camera
- `make run_mask` executes a live mask program to determine the best HSV filter to be applied

