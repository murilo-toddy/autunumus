<h1 align="center">Image Processing and SLAM</h1>

A scientific research project focused on characteristic extraction and mapping for
an autonomous vehicle.

# Contents

This repository is divided into three main modules: `Image Processing`, `ROS` and `SLAM`.
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
- Road edge detection algorithm
    - Using sampled images
    - Using live video

## [ROS](https://github.com/murilo-toddy/path-detection/tree/main/ros_ws)


### Contents

The `ros_ws` module contains a `ROS` simulation environment using `Gazebo` to extract
data to be further processed by the `SLAM` module.

- Differential Robot
    - LiDAR
    - Camera
    - Encoders
- Pre-built Gazebo world
- RViz robot status


## [SLAM](https://github.com/murilo-toddy/path-detection/tree/main/slam)

The `slam` module contains a `Python` and `Numpy` implementation of `FastSLAM 1.0` and `FastSLAM 2.0`.
It's able to simultaneously locate and map the robot.

### Contents

- Data extraction form `rosbag`
- FastSLAM 1.0 and 2.0 implementation
- User Interface for data visualization
