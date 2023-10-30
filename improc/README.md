# Image Processing

The image processing module is focused mainly on creating a deterministic algorithm to identify
multicolor cones in a custom scenario.

## Contents

- Camera calibration
- Color picking in HSV spectrum based on sampled images or input video
- Cone detection algorithm
    - Using sampled images
    - Using live video

## Configuration

The program can be configured by editing the `config.ini` file, that exports all constants
to the main application

### Camera Calibration

This camera calibration algorithm works based on chessboard detection. Add chessboard input
images to `

Calibration matrices will be printed to standard output once execution is finished. Matrices will be
printed in the standard output.

### Color Picking

A simple algorithm that allows for color filtering in HSV color space, based either in live
video or sampled images.

This allows for the user to easily determine witch colors will be used in the filtering
step of cone detection

### Cone Detection Algorithm

Detects cones present in the frame by usage of matrix transformations. Output will be
a list of cones in polar coordinates in the camera frame.

## Dependencies

This project is built using
- `Python 3.11`
- `OpenCV 4`

## Execution

To run the main application, simply setup your python env using

```bash
python3 -m venv venv
```

and install all dependencies using

```bash
pip install -r requirements.txt
```

Then run the application via

```bash
python3 src/main.py
```

