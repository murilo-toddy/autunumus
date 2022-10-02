import copy

from fastslam import FastSLAM
from file_handler import *
from landmark import *
from particle import Particle
from robot import *

# Robot constants
scanner_displacement = 0.3
robot_width = 0.4

# Cylinder extraction and estimation constants
minimum_valid_distance = 1
depth_jump = 4
cylinder_offset = 0.2

# Filter constants
control_motion_factor = 0.1  # Error in motor control.
control_turn_factor = 0.6  # Additional error due to slip when turning.
measurement_distance_stddev = 1  # Distance measurement error of cylinders.
measurement_angle_stddev = 1.5 / 180.0 * pi  # Angle measurement error.

# Minimum accepted correspondence likelihood
minimum_correspondence_likelihood = 1e-7

# Number of robot particles to be processed
# Linear complexity
number_of_particles = 50

if __name__ == '__main__':
    initial_pose = [4.953761677051627, 16.12791136345065, -0.4518609926531719]  # Pose defined for better visualization
    start_state = np.array(initial_pose)
    initial_particles = [copy.copy(Particle(start_state, robot_width, scanner_displacement))
                         for _ in range(number_of_particles)]

    # Setup filter.
    fs = FastSLAM(initial_particles,
                  robot_width, scanner_displacement,
                  control_motion_factor, control_turn_factor,
                  measurement_distance_stddev,
                  measurement_angle_stddev,
                  minimum_correspondence_likelihood)

    # Read data.
    robot_data = Robot()
    robot_data.read("./robot_data/motor.txt")
    robot_data.read("./robot_data/lidar.txt")

    # Loop over all motor tick records.
    with open("./robot_data/slam.txt", "w") as f:
        for i in range(len(robot_data.motor_ticks)):
            # Prediction step
            control = robot_data.motor_ticks[i]
            fs.predict(control)

            # Correction step
            cylinders = get_cylinders_from_scan(robot_data.scan_data[i], depth_jump,
                                                minimum_valid_distance, cylinder_offset)
            fs.correct(cylinders)

            # Get mean and variance for particles
            mean = get_mean(fs.particles)
            errors = get_error_ellipse_and_heading_variance(fs.particles, mean)

            # Output landmarks of particle which is closest to the mean position.
            output_particle = min([
                (np.linalg.norm(mean[0:2] - fs.particles[i].pose[0:2]), i)
                for i in range(len(fs.particles))])[1]

            # Write information to file
            write_particles(f, "PA", fs.particles)
            write_particle_pose(f, "F", mean, scanner_displacement)
            write_robot_variance(f, "E", errors)
            write_landmarks(f, "W C", fs.particles[output_particle].landmark_positions)
            write_error_ellipses(f, "W E", fs.particles[output_particle].landmark_covariances)
