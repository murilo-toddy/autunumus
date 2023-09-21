from dataclasses import dataclass
from math import sin, cos

import numpy as np

from transformations import Lidar


@dataclass
class Landmark:
    counter: int
    position: np.ndarray
    covariance: np.ndarray
#     def __init__(self, counter: int, position: np.ndarray, covariance: np.ndarray):
#         self.counter = counter
#         self.position = position
#         self.covariance = covariance

# Calculate derivative of a list, considering values greater than min_dist
def compute_derivative(scan: list[float], min_dist: float) -> list[float]:
    derivative_list = [0.0]
    for i in range(1, len(scan) - 1):
        left = scan[i-1]
        right = scan[i+1]
        if left > min_dist and right > min_dist:
            derivative = (right - left) / 2.0
            derivative_list.append(derivative)
        else:
            derivative_list.append(0)
    derivative_list.append(0)
    return derivative_list


# For each area between a left falling edge and a right rising edge,
# determine the average ray number and the average depth
def find_landmarks(scan: list[float], scan_derivative: list[float], jump: float, min_dist: float) \
        -> list[tuple[float, float]]:
    landmark_list = []
    on_landmark = False
    sum_ray, sum_depth, rays = 0.0, 0.0, 0
    for i in range(len(scan_derivative)):
        # Start a new landmark, independent of on_landmark
        if scan_derivative[i] < -jump:
            on_landmark = True
            sum_ray, sum_depth, rays = 0.0, 0.0, 0
        # Save landmark if there was one
        elif scan_derivative[i] > jump:
            if on_landmark and rays:
                landmark_list.append((sum_ray/rays, sum_depth/rays))
            on_landmark = False
        # Always add point, if it is a valid measurement
        elif scan[i] > min_dist:
            sum_ray += i
            sum_depth += scan[i]
            rays += 1
    return landmark_list


# Detect landmark and return its position in world coordinates, cartesian and polar
def get_landmarks_from_scan(scan: list[float], jump: float, min_dist: float, landmark_offset: float) \
        -> list[tuple[np.ndarray, np.ndarray]]:
    der = compute_derivative(scan, min_dist)
    landmarks = find_landmarks(scan, der, jump, min_dist)
    result = []
    for c in landmarks:
        # Compute the angle and distance measurements
        bearing = Lidar.beam_index_to_angle(c[0])
        distance = c[1] + landmark_offset
        # Compute x, y of landmark in the scanner system
        x, y = distance*cos(bearing), distance*sin(bearing)
        result.append((np.array([distance, bearing]), np.array([x, y])))
    return result

