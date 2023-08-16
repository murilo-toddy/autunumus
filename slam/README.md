# FASTSLAM 2.0

This module contains a `FastSLAM 2.0` implementation in `Python` and `Numpy`.
It receives vehicle's landmark and control data via `ZMQ` and processes them
in order to create a map of the space around it.

## Usage

To utilize this module, you may provide landmarks and vehicle control
data using the ports `5555` and `5556` of your source, respectively.
Also, the source must be connected to the same docker network as the slam module

Example with mocked encoder and LiDAR data is available at the `mock` folder.

## Setup 

You will need:

- `Python 3.10.7`
- `Docker`

To setup your machine and install all necessary dependencies, use:

```bash
python3 -m venv venv
. venv/bin/activate
pip install -r requirements.txt
```


## Configuration

First, you must create a local `Docker` network that will be used for the
SLAM algorithm to fetch data. You can do this using

```bash
make setup
```

To build all the relevant containers, use

```bash
make build
```

Then, to run the containers, use

```bash
make run
```

Output data will automatically be stored at a `logs.txt` file. It will also
be sent to an external API (refer to `telemetry-api` module) to allow
for telemetry visualization.

