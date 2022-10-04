from math import pi

from fastslam import FastSLAM
from fastslam2 import FastSLAM2
from file_handler import *
from landmark import get_landmarks_from_scan
from robot import *

# Robot constants
scanner_displacement = 0.3
robot_width = 0.4

# Landmark extraction and estimation constants
minimum_valid_distance = 1
depth_jump = 4
landmark_offset = 0.2

# Filter constants
control_motion_factor = 0.1  # Error in motor control
control_turn_factor = 0.6  # Additional error due to slip when turning
measurement_distance_stddev = 1  # Distance measurement error of landmark
measurement_angle_stddev = 1.5 / 180.0 * pi  # Angle measurement error

# Minimum accepted correspondence likelihood
minimum_correspondence_likelihood = 1e-7

# Number of robot particles to be processed
# Linear complexity
number_of_particles = 50


def fastslam(robot_data: Robot, slam, filename: str) -> None:
    # Slam algorithm setup
    fs = slam(start_state, number_of_particles,
                  robot_width, scanner_displacement,
                  control_motion_factor, control_turn_factor,
                  measurement_distance_stddev,
                  measurement_angle_stddev,
                  minimum_correspondence_likelihood)

    # Loop over all motor tick records.
    with open(f"./robot_data/{filename}", "w") as f:
        for i in range(len(robot_data.motor_ticks)):
            # Prediction step
            control = robot_data.motor_ticks[i]
            fs.predict(control)

            # Correction step
            landmarks = get_landmarks_from_scan(robot_data.scan_data[i], depth_jump,
                                                minimum_valid_distance, landmark_offset)
            fs.correct(landmarks)

            # Get mean and variance for particles
            mean = fs.get_mean()
            errors = fs.get_error_ellipse_and_heading_variance()

            # Output landmarks of particle which is closest to the mean position.
            output_particle = min([
                (np.linalg.norm(mean[0:2] - fs.particles[i].pose[0:2]), i)
                for i in range(len(fs.particles))])[1]

            # Write information to file
            write_particles(f, "PA", fs.particles)
            write_particle_pose(f, "F", mean, scanner_displacement)
            write_robot_variance(f, "E", errors)
            write_landmarks(f, "W C", fs.particles[output_particle].landmarks)
            write_error_ellipses(f, "W E", fs.particles[output_particle].landmarks)

if __name__ == '__main__':
    initial_pose = [4.953761677051627, 16.12791136345065, -0.4518609926531719]  # Pose defined for better visualization
    start_state = np.array(initial_pose)

    # Read data.
    robot_data = Robot()
    robot_data.read("./robot_data/motor.txt")
    robot_data.read("./robot_data/lidar.txt")

    fastslam(robot_data, FastSLAM, "fastslam.txt")
    fastslam(robot_data, FastSLAM2, "fastslam2.txt")
