import requests
from math import pi

from fastslam import FastSLAM
from file_handler import *
from zmqprovider import ZMQProvider

# Robot constants
scanner_displacement = 0.3
robot_width = 0.4

# Landmark extraction and estimation constants
minimum_valid_distance = 1
depth_jump = 4
landmark_ofslamet = 0.2

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

# Pose defined for better visualization
initial_pose = [4.953761677051627, 16.12791136345065, -0.4518609926531719]  


if __name__ == '__main__':
    # requests.post(url, json={"number": 1})

    start_state = np.array(initial_pose)

    # Slam algorithm setup
    slam = FastSLAM(start_state, number_of_particles,
                  robot_width, scanner_displacement,
                  control_motion_factor, control_turn_factor,
                  measurement_distance_stddev,
                  measurement_angle_stddev,
                  minimum_correspondence_likelihood)

    zmq_provider = ZMQProvider()

    while True:
        # Prediction step
        control, landmarks = zmq_provider.read_data()
        if control is None or landmarks is None:
            print("timeout")
            break 
        
        # TODO: main file structure should be:
        # out = fs.cycle(control, landmarks) 
        # post_to_telemetry_api(out)

        slam.predict(control)

        # Correction step
        slam.correct(landmarks)

        # Get mean and variance for particles
        mean = slam.get_mean()
        errors = slam.get_error_ellipse_and_heading_variance()

        # Output landmarks of particle which is closest to the mean position.
        output_particle = min([
            (np.linalg.norm(mean[0:2] - slam.particles[i].pose[0:2]), i)
            for i in range(len(slam.particles))])[1]

        # Write information to file
        # write_particles(f, "PA", slam.particles)
        # write_particle_pose(f, "F", mean, scanner_displacement)
        # write_robot_variance(f, "E", errors)
        # write_landmarks(f, "W C", slam.particles[output_particle].landmarks)
        # write_error_ellipses(f, "W E", slam.particles[output_particle].landmarks)

