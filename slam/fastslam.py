import copy
import random
from math import sqrt, pi, atan2, sin, cos

import numpy as np


class FastSLAM:
    def __init__(self, initial_particles,
                 robot_width, scanner_displacement,
                 control_motion_factor, control_turn_factor,
                 measurement_distance_stddev, measurement_angle_stddev,
                 minimum_correspondence_likelihood):
        self.particles = initial_particles

        self.mean = []
        self.robot_width = robot_width
        self.scanner_displacement = scanner_displacement
        self.control_motion_factor = control_motion_factor
        self.control_turn_factor = control_turn_factor
        self.measurement_distance_stddev = measurement_distance_stddev
        self.measurement_angle_stddev = measurement_angle_stddev
        self.minimum_correspondence_likelihood = \
            minimum_correspondence_likelihood

    def predict(self, control: tuple[float, float]) -> None:
        """ Prediction step """
        left, right = control
        left_std = sqrt((self.control_motion_factor * left) ** 2 +
                        (self.control_turn_factor * (left - right)) ** 2)
        right_std = sqrt((self.control_motion_factor * right) ** 2 +
                         (self.control_turn_factor * (left - right)) ** 2)

        # Move each particle
        for p in self.particles:
            post_l = random.gauss(left, left_std)
            post_r = random.gauss(right, right_std)
            p.move(post_l, post_r)

    def update_and_compute_weights(self, landmarks):
        """ Update all particles and return their weights """
        measurement_intrinsic_covariance = \
            np.diag([self.measurement_distance_stddev ** 2,
                     self.measurement_angle_stddev ** 2])
        weights = []
        for p in self.particles:
            # Added: decrement landmark counter for any landmark that should be
            # visible.
            p.decrement_visible_landmark_counters(self.scanner_displacement)

            # Loop over all measurements.
            number_of_landmarks = p.number_of_landmarks()
            weight = 1.0
            for measurement, measurement_in_scanner_system in landmarks:
                weight *= p.update_particle(
                    measurement, measurement_in_scanner_system,
                    number_of_landmarks,
                    self.minimum_correspondence_likelihood,
                    measurement_intrinsic_covariance, self.scanner_displacement)

            # Append overall weight of this particle to weight list.
            weights.append(weight)

            # Added: remove spurious landmarks (with negative counter).
            p.remove_spurious_landmarks()

        return weights

    def resample(self, weights):
        """Return a list of particles which have been resampled, proportional
           to the given weights."""
        new_particles = []
        max_weight = max(weights)
        index = random.randint(0, len(self.particles) - 1)
        offset = 0.0
        for i in range(len(self.particles)):
            offset += random.uniform(0, 2.0 * max_weight)
            while offset > weights[index]:
                offset -= weights[index]
                index = (index + 1) % len(weights)
            new_particles.append(copy.deepcopy(self.particles[index]))
        return new_particles

    def correct(self, landmarks):
        """The correction step of FastSLAM."""
        # Update all particles and compute their weights.
        weights = self.update_and_compute_weights(landmarks)
        # Then resample, based on the weight array.
        self.particles = self.resample(weights)

    # Compute mean position and heading from a given set of particles
    def get_mean(self):
        mean_x, mean_y = 0.0, 0.0
        head_x, head_y = 0.0, 0.0
        for p in self.particles:
            x, y, theta = p.pose
            mean_x += x
            mean_y += y
            head_x += cos(theta)
            head_y += sin(theta)
        n = max(1, len(self.particles))
        self.mean = np.array([mean_x / n, mean_y / n, atan2(head_y, head_x)])
        return self.mean

    # Given a set of particles and their mean, returns orientation of xy error ellipse
    def get_error_ellipse_and_heading_variance(self):
        center_x, center_y, center_heading = self.mean
        n = len(self.particles)
        if n < 2:
            return 0.0, 0.0, 0.0, 0.0

        # Compute covariance matrix in xy
        sxx, sxy, syy = 0.0, 0.0, 0.0
        for p in self.particles:
            x, y, _ = p.pose
            dx = x - center_x
            dy = y - center_y
            sxx += dx * dx
            sxy += dx * dy
            syy += dy * dy
        cov_xy = np.array([[sxx, sxy], [sxy, syy]]) / (n - 1)

        # Get variance of heading
        var_heading = 0.0
        for p in self.particles:
            dh = (p.pose[2] - center_heading + pi) % (2 * pi) - pi
            var_heading += dh * dh
        var_heading = var_heading / (n - 1)

        # Convert xy to error ellipse
        eigenvals, eigenvects = np.linalg.eig(cov_xy)
        ellipse_angle = atan2(eigenvects[1, 0], eigenvects[0, 0])

        return (ellipse_angle, sqrt(abs(eigenvals[0])),
                sqrt(abs(eigenvals[1])),
                sqrt(var_heading))
