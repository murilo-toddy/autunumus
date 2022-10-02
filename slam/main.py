from robot import *
from cylinder import *
from math import pi
from transformations import *
from file_handler import *
import copy
import numpy as np
from particle import Particle
from fastslam import FastSLAM


if __name__ == '__main__':
    # Robot constants.
    scanner_displacement = 0.3
    ticks_to_mm = 1
    robot_width = 0.4

    # Cylinder extraction and matching constants.
    minimum_valid_distance = 1
    depth_jump = 4
    cylinder_offset = 0.2

    # Filter constants.
    control_motion_factor = 0.1  # Error in motor control.
    control_turn_factor = 0.6  # Additional error due to slip when turning.
    measurement_distance_stddev = 1  # Distance measurement error of cylinders.
    measurement_angle_stddev = 1.5 / 180.0 * pi  # Angle measurement error.
    minimum_correspondence_likelihood = 1e-7  # Min likelihood of correspondence.

    # Generate initial particles. Each particle is (x, y, theta).
    number_of_particles = 50
    # start_state = np.array([20.0, 20.0, 45.0 / 180.0 * pi])
    start_state = np.array([4.953761677051627, 16.12791136345065, -0.4518609926531719])
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
    logfile = Robot()
    logfile.read("./robot_data/motor.txt")
    logfile.read("./robot_data/lidar.txt")

    # Loop over all motor tick records.
    # This is the FastSLAM filter loop, with prediction and correction.
    f = open("./robot_data/slam.txt", "w")
    for i in range(len(logfile.motor_ticks)):
        # Prediction.
        control = map(lambda x: x * ticks_to_mm, logfile.motor_ticks[i])
        fs.predict(control)

        # Correction.
        cylinders = get_cylinders_from_scan(logfile.scan_data[i], depth_jump,
            minimum_valid_distance, cylinder_offset)
        # print(cylinders)
        fs.correct(cylinders)

        # Output particles.
        # print_particles(fs.particles, f)
        write_particles(f, "PA", fs.particles)

        # Output state estimated from all particles.
        mean = get_mean(fs.particles)
        write_particle_pose(f, "F", mean, scanner_displacement)
        # f.write("F %f %f %f\n" %\
        #       (mean[0] + scanner_displacement * cos(mean[2]),
        #        mean[1] + scanner_displacement * sin(mean[2]),
        #        mean[2]))

        # Output error ellipse and standard deviation of heading.
        errors = get_error_ellipse_and_heading_variance(fs.particles, mean)
        write_robot_variance(f, "E", errors)

        # Output landmarks of particle which is closest to the mean position.
        output_particle = min([
            (np.linalg.norm(mean[0:2] - fs.particles[i].pose[0:2]),i)
            for i in range(len(fs.particles)) ])[1]
        # Write estimates of landmarks.
        write_landmarks(f, "W C",
                        fs.particles[output_particle].landmark_positions)
        # Write covariance matrices.
        write_error_ellipses(f, "W E",
                             fs.particles[output_particle].landmark_covariances)

    f.close()
