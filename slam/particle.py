from math import pi, sqrt, exp, atan2

from landmark import Landmark

from transformations import *


class Particle:
    def __init__(self, pose: np.ndarray, robot_width: float, scanner_displacement: float):
        self.pose = pose
        self.w = robot_width
        self.scanner_displacement = scanner_displacement
        self.landmarks = []

    def number_of_landmarks(self) -> int:
        """ Number of detected landmarks """
        return len(self.landmarks)
    
    def move(self, left: float, right: float) -> None:
        """ Move particle by given control """
        self.pose = self.state_transition(self.pose, (left, right), self.w)

    @staticmethod
    def state_transition(state: np.ndarray, control: tuple, w: float) -> np.ndarray:
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
    def state_transition_jacobian(state: np.ndarray, landmark: tuple[float, float], scanner_displacement: float) \
            -> np.ndarray:
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
    def measurement_function(state: np.ndarray, landmark: list[np.ndarray], scanner_displacement: float) \
            -> np.ndarray:
        """ Calculate range and bearing from state and landmark """
        dx = landmark[0] - (state[0] + scanner_displacement * cos(state[2]))
        dy = landmark[1] - (state[1] + scanner_displacement * sin(state[2]))
        r = sqrt(dx * dx + dy * dy)
        alpha = (atan2(dy, dx) - state[2] + pi) % (2*pi) - pi
        return np.array([r, alpha])

    def expected_pose_measurement_for_landmark(self, landmark: int) -> np.array:
        """ Calculate expected distance and angle for specific landmark """
        return self.measurement_function(self.pose,
                                         self.landmarks[landmark].position,
                                         self.scanner_displacement)
    
    def state_and_measurement_covariance_jacobians(self, landmark: int, intrinsic_covariance: np.ndarray) \
            -> tuple[np.ndarray, np.ndarray]:
        """ Compute jacobian of state position and measurement covariance matrix """
        state_transition_jacobian = self.state_transition_jacobian(
            self.pose, self.landmarks[landmark].position,
            self.scanner_displacement)
        covariance_matrix = np.dot(
            np.dot(state_transition_jacobian, self.landmarks[landmark].covariance),
            state_transition_jacobian.T) + intrinsic_covariance
        return state_transition_jacobian, covariance_matrix

    def estimate_likelihood_of_correspondence(self, measurement: np.ndarray, landmark: int,
                                              intrinsic_covariance: np.ndarray) \
            -> float:
        """ Estimate likelihood of correspondence that a given measurement corresponds
            to the landmark """
        delta_z = measurement - self.expected_pose_measurement_for_landmark(landmark)
        _, covariance_matrix = self.state_and_measurement_covariance_jacobians(landmark, intrinsic_covariance)
        return 1 / (2*pi*sqrt(np.linalg.det(covariance_matrix))) * \
            exp(-1/2 * np.dot(delta_z.T, np.dot(np.linalg.inv(covariance_matrix), delta_z)))

    def compute_correspondence_likelihoods(self, measurement: np.ndarray,
                                           landmarks: int,
                                           intrinsic_covariance: np.ndarray)\
            -> list[float]:
        """ Return a list of correspondence likelihoods for a given measurement """
        likelihoods = []
        for i in range(landmarks):
            likelihoods.append(
                self.estimate_likelihood_of_correspondence(
                    measurement, i, intrinsic_covariance))
        return likelihoods

    def initialize_new_landmark(self, measurement_in_scanner_system: list[np.ndarray],
                                intrinsic_covariance: np.ndarray) -> None:
        """ Initialize new landmark given its measurement in world coordinates """
        scanner_pose = (self.pose[0] + cos(self.pose[2]) * self.scanner_displacement,
                        self.pose[1] + sin(self.pose[2]) * self.scanner_displacement,
                        self.pose[2])
        m = scanner_to_world(scanner_pose, measurement_in_scanner_system)
        state_jacobian = self.state_transition_jacobian(self.pose, m, self.scanner_displacement)
        state_jacobian_inverse = np.linalg.inv(state_jacobian)
        sigma = np.dot(state_jacobian_inverse, np.dot(intrinsic_covariance, state_jacobian_inverse.T))
        self.landmarks.append(Landmark(1, np.array(m), sigma))

    def update_landmark(self, landmark: int, measurement: np.ndarray,
                        intrinsic_covariance: np.ndarray) -> None:
        """ Update landmark position and covariance """
        state_jacobian, covariance_matrix = \
            self.state_and_measurement_covariance_jacobians(landmark, intrinsic_covariance)
        k = np.dot(self.landmarks[landmark].covariance, np.dot(state_jacobian.T, np.linalg.inv(covariance_matrix)))
        delta_z = measurement - self.expected_pose_measurement_for_landmark(landmark)
        self.landmarks[landmark].position = self.landmarks[landmark].position + np.dot(k, delta_z)
        self.landmarks[landmark].covariance = \
            np.dot(np.eye(2) - np.dot(k, state_jacobian), self.landmarks[landmark].covariance)
        self.landmarks[landmark].counter += 2

    def update_particle(self, measurement: np.ndarray, measurement_in_scanner_system: list[np.ndarray],
                        landmarks: int, minimum_correspondence_likelihood: float,
                        intrinsic_covariance: np.ndarray) -> float:
        """ Calculate likelihood that a measurement belongs to any landmark observed 
            If true, update landmark EKF and add a new landmark otherwise """
        likelihoods = self.compute_correspondence_likelihoods(measurement, landmarks, intrinsic_covariance)
        # If there is not a correspondence, insert new landmark
        if not likelihoods or\
           max(likelihoods) < minimum_correspondence_likelihood:
            self.initialize_new_landmark(measurement_in_scanner_system, intrinsic_covariance)
            return minimum_correspondence_likelihood

        # Else update landmark EKF
        else:
            w = max(likelihoods)
            index = likelihoods.index(w)
            self.update_landmark(index, measurement, intrinsic_covariance)
            return w

    def decrement_visible_landmark_counters(self) -> None:
        """ Decrement counter of every landmark that should have been observed """
        min_ang, max_ang = Lidar.min_max_bearing()
        for i in range(len(self.landmarks)):
            d, ang = self.expected_pose_measurement_for_landmark(i)
            """ Not Implemented """
            # if not(min_ang/2 < ang and ang < max_ang/2) and d < 20:
            #     self.landmark_counters[i] -= 0.1

    def remove_spurious_landmarks(self) -> None:
        """ Remove all landmarks with negative counter """
        self.landmarks = [landmark for landmark in self.landmarks if landmark.counter >= 0]
