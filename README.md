# AuTUnumus

Repository used to condense all code related to Tupã's autonomous vehicle adventure.

# Contents

This repository is divided into four main modules: `Image Processing`, `SLAM`, `ROS` and `Telemetry-Api`.
Specific information about how to execute each module is specified in each module's `README`.

## [Image Processing](https://github.com/murilo-toddy/autunumus/tree/main/improc)

The `improc` module contains a `Python` algorithm for cone identification. It's built using
the library `OpenCV`.

### Contents

- Camera calibration
- Color picking in HSV spectrum based on sampled images or input video
- Cone detection algorithm
    - Using input images
    - Using live video
- ZMQ connection with SLAM

## [SLAM](https://github.com/murilo-toddy/autunumus/tree/main/slam)

The `slam` module contains a `Python` and `Numpy` implementation of `FastSLAM 2.0`.
It's able to simultaneously locate and map the robot.

### Contents

- FastSLAM 2.0 implementation
- ZMQ connection to fetch input data
- API handler to pass information to our telemetry-api

## [ROS](https://github.com/murilo-toddy/autunumus/tree/main/ros_ws)

The `ros` module contains a simulation environment built using ROS Noetic. It contains
a differential robot implementation and allows testing of the previously mentioned systems
It's able to simultaneously locate and map the robot.

## [Telemetry-API](https://github.com/murilo-toddy/autunumus/tree/main/telemetry-api)

The `telemetry-api` contains a simple REST API built using Flask to receive data 
from our vehicle. It's used for real-time data visualization.

