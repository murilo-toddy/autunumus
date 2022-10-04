<h1 align="center">ROS Environment</h1>

This module defines a ROS simulation using Gazebo and RViz. Its main objective is to collect
input data from a robot to feed the SLAM algorithm.

The repository contains a robot containing a camera and a LiDAR, and it's based on `atom` 
and its original version can be found in 
[harshmittal2210](https://github.com/harshmittal2210/Robotics_ws)'s repository.

The robot is spawned in a pre-defined Gazebo map containing walls and landmarks, and it
can be controlled via `teleop_twist_keyboard`.

## Contents

- Differential Robot
  - LiDAR
  - Camera
  - Encoders
- Pre-built Gazebo world
- RViz robot status

## Configuration

This project was built using `Ubuntu 20.04` and `Ros Noetic`.
To build the repository, in the `ros_ws` folder run:

```bash
catkin_make
```

Add project to bash

```bash
source devel/setup.bash
```

## Dependencies

- `Ubuntu 20.04`
- `Ros Noetic`
- `Gazebo` and `RViz`
- `Python 3.10.7`
- `C++17`
- `CMake 3.18.4`

ROS plugins and libraries necessary to run the project can be installed via `rosdep`.

```bash
rosdep install --from-paths src --ignore-src -r -y
```

## Execution

The robot can be instantiated using:

```bash
roslaunch atom slam.launch
```

and controlled via

```bash
rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=/atom/cmd_vel
```

Data to be further processed can be saved using a `rosbag`. Topics necessary are:

- `/scan`
- `/atom/odom`
- `/atom/camera/rgb/image_raw` [Optional]