from math import pi, sqrt, exp, atan2

import numpy as np

from transformations import *


class Particle:
    def __init__(self, pose: np.ndarray, robot_width: float, scanner_displacement: float):
        self.pose = pose
        self.w = robot_width
        self.scanner_displacement = scanner_displacement
        self.landmarks = []
        # TODO update to dictionary
        self.landmark_positions = []
        self.landmark_covariances = []
        self.landmark_counters = []

    def number_of_landmarks(self):
        """ Number of detected landmarks """
        return len(self.landmark_positions)
    
    def move(self, left, right):
        """ Move particle by given control """
        self.pose = self.state_transition(self.pose, (left, right), self.w)

    @staticmethod
    def state_transition(state: np.ndarray, control: tuple, w: float) -> np.array:
        """ Moving model for robot given control u """
        x, y, theta = state
        l, r = control
        if r != l:
            alpha = (r - l) / w
            rad = l / alpha
            g1 = x + (rad + w / 2)*(sin(theta + alpha) - sin(theta))
            g2 = y + (rad + w / 2)*(-cos(theta + alpha) + cos(theta))
            g3 = (theta + alpha + pi) % (2*pi) - pi
            return np.array([g1, g2, g3])
        else:
            return np.array([
                x + l * cos(theta),
                y + l * sin(theta),
                theta
            ])

    @staticmethod
    def state_transition_jacobian(state: list, landmark: list, scanner_displacement: float) -> np.array:
        theta = state[2]
        cost, sint = cos(theta), sin(theta)
        dx = landmark[0] - (state[0] + scanner_displacement * cost)
        dy = landmark[1] - (state[1] + scanner_displacement * sint)
        q = dx * dx + dy * dy
        sqrtq = sqrt(q)
        dr_dmx = dx / sqrtq
        dr_dmy = dy / sqrtq
        dalpha_dmx = -dy / q
        dalpha_dmy = dx / q
        return np.array([[dr_dmx, dr_dmy],
                         [dalpha_dmx, dalpha_dmy]])

    @staticmethod
    def h(state, landmark, scanner_displacement):
        """Measurement function. Takes a (x, y, theta) state and a (x, y)
           landmark, and returns the corresponding (range, bearing)."""
        dx = landmark[0] - (state[0] + scanner_displacement * cos(state[2]))
        dy = landmark[1] - (state[1] + scanner_displacement * sin(state[2]))
        r = sqrt(dx * dx + dy * dy)
        alpha = (atan2(dy, dx) - state[2] + pi) % (2*pi) - pi
        return np.array([r, alpha])

    def expected_pose_measurement_for_landmark(self, landmark: int) -> np.array:
        """ Calculate expected distance and angle for specific landmark """
        return self.h(self.pose,
                                     self.landmark_positions[landmark],
                                     self.scanner_displacement)
    
    def state_and_measurement_covariance_jacobians(
        self, landmark, intrinsic_covariance):
        """ Compute jacobian of state position and measurement covariance matrix """
        state_transition_jacobian = self.state_transition_jacobian(
            self.pose, self.landmark_positions[landmark], 
            self.scanner_displacement)
        covariance_matrix = np.dot(
            np.dot(state_transition_jacobian, self.landmark_covariances[landmark]),
            state_transition_jacobian.T) + intrinsic_covariance
        return (state_transition_jacobian, covariance_matrix)

    def estimate_likelihood_of_correspondence(
        self, measurement, landmark, intrinsic_covariance):
        """ Estimate likelihood of correspondance that a given measurement corresponds
            to the landmark """
        delta_z = measurement - self.expected_pose_measurement_for_landmark(landmark)
        H, Ql = self.state_and_measurement_covariance_jacobians(landmark, intrinsic_covariance)
        l = 1 / (2*pi*sqrt(np.linalg.det(Ql))) * exp(-1/2 * np.dot(delta_z.T, np.dot(np.linalg.inv(Ql), delta_z)))
        return l

    def compute_correspondence_likelihoods(self, measurement,
                                           number_of_landmarks,
                                           Qt_measurement_covariance,
                                           scanner_displacement):
        """For a given measurement, returns a list of all correspondence
           likelihoods (from index 0 to number_of_landmarks-1)."""
        likelihoods = []
        for i in range(number_of_landmarks):
            likelihoods.append(
                self.estimate_likelihood_of_correspondence(
                    measurement, i, Qt_measurement_covariance))
        return likelihoods



    def initialize_new_landmark(self, measurement_in_scanner_system,
                                Qt_measurement_covariance):
        """Given a (x, y) measurement in the scanner's system, initializes a
           new landmark and its covariance."""
        scanner_pose = (self.pose[0] + cos(self.pose[2]) * self.scanner_displacement,
                        self.pose[1] + sin(self.pose[2]) * self.scanner_displacement,
                        self.pose[2])
        # --->>> Insert your code here.
        # Hints:
        # - LegoLogfile.scanner_to_world() (from lego_robot.py) will return
        #   the world coordinate, given the scanner pose and the coordinate in
        #   the scanner's system.
        m = scanner_to_world(scanner_pose, measurement_in_scanner_system)
        H = self.state_transition_jacobian(self.pose, m, self.scanner_displacement)
        Hinv = np.linalg.inv(H)
        sigma = np.dot(Hinv, np.dot(Qt_measurement_covariance, Hinv.T))
        # - H is obtained from dh_dlandmark()
        # - Use np.linalg.inv(A) to invert matrix A
        # - As usual, np.dot(A,B) is the matrix product of A and B.
        self.landmark_positions.append(np.array(m))  # Replace this.
        self.landmark_covariances.append(sigma)  # Replace this.

    def update_landmark(self, landmark_number, measurement,
                        Qt_measurement_covariance, scanner_displacement):
        """Update a landmark's estimated position and covariance."""
        # --->>> Insert your new code here.
        # Hints:
        # - H and Ql can be computed using
        #   H_Ql_jacobian_and_measurement_covariance_for_landmark()
        # - Use np.linalg.inv(A) to compute the inverse of A
        # - Delta z is measurement minus expected measurement
        # - Expected measurement can be computed using
        #   h_expected_measurement_for_landmark()
        # - Remember to update landmark_positions[landmark_number] as well
        #   as landmark_covariances[landmark_number].
        H, Ql = self.state_and_measurement_covariance_jacobians(landmark_number, Qt_measurement_covariance)
        k = np.dot(self.landmark_covariances[landmark_number], np.dot(H.T, np.linalg.inv(Ql)))
        delta_z = measurement - self.expected_pose_measurement_for_landmark(landmark_number)
        self.landmark_positions[landmark_number] = self.landmark_positions[landmark_number] + np.dot(k, delta_z)
        self.landmark_covariances[landmark_number] = np.dot(np.eye(2) - np.dot(k, H), self.landmark_covariances[landmark_number])

    def update_particle(self, measurement, measurement_in_scanner_system,
                        number_of_landmarks,
                        minimum_correspondence_likelihood,
                        Qt_measurement_covariance, scanner_displacement):
        """Given a measurement, computes the likelihood that it belongs to any
           of the landmarks in the particle. If there are none, or if all
           likelihoods are below the minimum_correspondence_likelihood
           threshold, add a landmark to the particle. Otherwise, update the
           (existing) landmark with the largest likelihood."""
        # --->>> Insert your code below, at the marked locations.
        
        # Compute likelihood of correspondence of measurement to all landmarks
        # (from 0 to number_of_landmarks-1).
        likelihoods = self.compute_correspondence_likelihoods(measurement, number_of_landmarks, Qt_measurement_covariance, self.scanner_displacement)

        # If the likelihood list is empty, or the max correspondence likelihood
        # is still smaller than minimum_correspondence_likelihood, setup
        # a new landmark.
        # if likelihoods: print(likelihoods)
        if not likelihoods or\
           max(likelihoods) < minimum_correspondence_likelihood:
            self.initialize_new_landmark(measurement_in_scanner_system, Qt_measurement_covariance)
            self.landmark_counters.append(1)
            # --->>> Add code to insert a new landmark.
            return minimum_correspondence_likelihood

        # Else update the particle's EKF for the corresponding particle.
        else:
            # This computes (max, argmax) of measurement_likelihoods.
            w = max(likelihoods)
            index = likelihoods.index(w)
            self.landmark_counters[index] += 2
            
            # --->>> Add code to find w, the maximum likelihood,
            self.update_landmark(index, measurement, Qt_measurement_covariance, scanner_displacement)
            # and the corresponding landmark index.
            # w, index = 0.001, 0  # Replace this by a search for max, argmax.
            # Add code to update_landmark().
            return w

    # Added: Counter decrement for visible landmarks.
    def decrement_visible_landmark_counters(self, scanner_displacement):
        """Decrements the counter for every landmark which is potentially
           visible. This uses a simplified test: it is only checked if the
           bearing of the expected measurement is within the laser scanners
           range."""
        # --->>> Insert your new code here.
        # Hints:
        min_ang, max_ang = Lidar.min_max_bearing()
        for i in range(len(self.landmark_positions)):
            d, ang = self.expected_pose_measurement_for_landmark(i)
            """ Not Implemented """
            # if not(min_ang/2 < ang and ang < max_ang/2) and d < 20:
            #     self.landmark_counters[i] -= 0.1
        # - We only check the bearing angle of the landmarks.
        # - Min and max bearing can be obtained from
        #   LegoLogfile.min_max_bearing()
        # - The bearing for any landmark can be computed using
        #   h_expected_measurement_for_landmark()
        # - If the bearing is within the range, decrement the corresponding
        #   self.landmark_counters[].

    # Added: Removal of landmarks with negative counter.
    def remove_spurious_landmarks(self):
        """Remove all landmarks which have a counter less than zero."""
        self.landmark_counters = [c for c in self.landmark_counters if c >= 0]
        self.landmark_covariances = [cov for cov, count in zip(self.landmark_covariances, self.landmark_counters) if count >= 0]
        self.landmark_positions = [pos for pos, count in zip(self.landmark_positions, self.landmark_counters) if count >= 0]
        # --->>> Insert your new code here.
        # Remove any landmark for which the landmark_counters[] is (strictly)
        # smaller than zero.
        # Note: deleting elements of a list while iterating over the list
        # will not work properly. One relatively simple and elegant solution is
        # to make a new list which contains all required elements (c.f. list
        # comprehensions with if clause).
        # Remember to process landmark_positions, landmark_covariances and
        # landmark_counters.