# AuTUnumus

Repository used to condense all code related to Tupã's autonomous vehicle adventure.

# Contents

This repository is divided into three main modules: `Image Processing`, `SLAM` and `Telemetry-Api`.
Specific information about how to execute each module is specified in each module's `README`.

## [Image Processing](https://github.com/murilo-toddy/path-detection/tree/main/improc)

The `improc` module contains a `C++`algorithm for cone identification. It's built using
the library `OpenCV`.

### Contents

- Camera calibration
- Color picking in HSV spectrum based on sampled images or input video
- Deterministic cone detection algorithm
    - Using sampled images
    - Using live video

## [SLAM](https://github.com/murilo-toddy/path-detection/tree/main/slam)

The `slam` module contains a `Python` and `Numpy` implementation of `FastSLAM 2.0`.
It's able to simultaneously locate and map the robot.

### Contents

- FastSLAM 2.0 implementation
- ZMQ connection to fetch input data
- API handler to pass information to our telemetry-api

## [Telemetry-API](https://github.com/murilo-toddy/path-detection/tree/main/telemetry-api)

The `telemetry-api` contains a simple REST API to receive data from our vehicle. It's used
for real-time data visualization.

### Contents

- None for now :(
