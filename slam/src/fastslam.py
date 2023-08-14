import copy
import random
from math import sqrt, pi, atan2, sin, cos

import numpy as np

from particle import Particle


class FastSLAM:
    def __init__(self, start_state, number_of_particles,
                 robot_width, scanner_displacement,
                 control_motion_factor, control_turn_factor,
                 measurement_distance_stddev, measurement_angle_stddev,
                 minimum_correspondence_likelihood):
        self.mean = []
        self.robot_width = robot_width
        self.scanner_displacement = scanner_displacement
        self.control_motion_factor = control_motion_factor
        self.control_turn_factor = control_turn_factor
        self.measurement_distance_stddev = measurement_distance_stddev
        self.measurement_angle_stddev = measurement_angle_stddev
        self.minimum_correspondence_likelihood = \
            minimum_correspondence_likelihood

        self.particles = self._get_initial_particles(start_state, number_of_particles)

    def _get_initial_particles(self, start_state, number_of_particles):
        """ Load initial array of particles """
        return [copy.copy(Particle(start_state, self.robot_width, self.scanner_displacement))
                for _ in range(number_of_particles)]

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

    def update_particles_and_compute_weights(self, landmarks: list[tuple[np.ndarray, np.ndarray]]) \
            -> list[float]:
        """ Update all particles and return their weights """
        measurement_intrinsic_covariance = \
            np.diag([self.measurement_distance_stddev ** 2,
                     self.measurement_angle_stddev ** 2])

        weights = []
        for p in self.particles:
            # Decrement particle counter if it should have been observed
            p.decrement_visible_landmark_counters()

            # Loop over all measurements
            number_of_landmarks = p.number_of_landmarks()
            weight = 1.0
            for measurement, measurement_in_scanner_system in landmarks:
                weight *= p.update_particle(
                    measurement, measurement_in_scanner_system,
                    number_of_landmarks,
                    self.minimum_correspondence_likelihood,
                    measurement_intrinsic_covariance)

            # Append overall weight of this particle to weight list
            weights.append(weight)

            # Delete landmarks with negative counter
            p.remove_spurious_landmarks()

        return weights

    def resample(self, weights: list[float]) -> list[Particle]:
        """ Return resampled particles from calculated weights """
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

    def correct(self, landmarks: list[tuple[np.ndarray, np.ndarray]]) -> None:
        """ Correction step """
        weights = self.update_particles_and_compute_weights(landmarks)
        self.particles = self.resample(weights)

    def get_mean(self):
        """ Calculate mean position and heading for every particle """
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

    def get_error_ellipse_and_heading_variance(self) -> tuple[float, float, float, float]:
        """ Calculate orientation of xy error ellipse for each particle """
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

