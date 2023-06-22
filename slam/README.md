<h1 align="center">Simultaneous Location and Mapping</h1>

This module contains a `FastSLAM 1.0` and `2.0` implementation in `Python` and `Numpy`.
It takes as input robot's left and right wheel encoders and LiDAR data. Further configuration
about the setup can be updated in each `python` file.

## Contents

- Data extraction form `rosbag`
- FastSLAM 1.0 and 2.0 implementation
- User Interface for data visualization

## Configuration

To extract data from `rosbag`, use `robot_data/extract_data.py` updating the path to the file

SLAM script takes as input data stored in `robot_data/lidar.txt` and `robot_data/motor.txt` to
run properly. Robot's real path is stored in `robot_data/odom.txt`.

## Dependencies

- `Python 3.10.7`
- `Numpy`
- `TKinter`

Python dependencies can be installed via pip using

```bash
python3 -m pip install -r requirements.txt
```

## Execution

To run the SLAM calculations, use

```bash
python3 main.py
```

To open the User Interface, use

```bash
python3 ui/data_view.py
```

By default, SLAM's output data is saved into `robot_data/fastslamX.txt`

