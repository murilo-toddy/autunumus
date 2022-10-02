from dataclasses import dataclass
from math import sin, cos

import numpy as np

from transformations import Lidar


@dataclass
class Landmark:
    counter: int
    position: list[np.ndarray]
    covariance: list[np.ndarray]


# Calculate derivative of a list, considering values greater than min_dist
def compute_derivative(scan: list[float], min_dist: float) -> list[float]:
    derivative_list = [0]
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
def find_cylinders(scan: list[float], scan_derivative: list[float], jump: float, min_dist: float) \
        -> list[tuple[float, float]]:
    cylinder_list = []
    on_cylinder = False
    sum_ray, sum_depth, rays = 0.0, 0.0, 0
    for i in range(len(scan_derivative)):
        # Start a new cylinder, independent of on_cylinder
        if scan_derivative[i] < -jump:
            on_cylinder = True
            sum_ray, sum_depth, rays = 0.0, 0.0, 0
        # Save cylinder if there was one
        elif scan_derivative[i] > jump:
            if on_cylinder and rays:
                cylinder_list.append((sum_ray/rays, sum_depth/rays))
            on_cylinder = False
        # Always add point, if it is a valid measurement
        elif scan[i] > min_dist:
            sum_ray += i
            sum_depth += scan[i]
            rays += 1
    return cylinder_list


# Detect cylinder and return its position in world coordinates, cartesian and polar
def get_cylinders_from_scan(scan: list[float], jump: float, min_dist: float, cylinder_offset: float) \
        -> list[tuple[np.ndarray, np.ndarray]]:
    der = compute_derivative(scan, min_dist)
    cylinders = find_cylinders(scan, der, jump, min_dist)
    result = []
    for c in cylinders:
        # Compute the angle and distance measurements
        bearing = Lidar.beam_index_to_angle(c[0])
        distance = c[1] + cylinder_offset
        # Compute x, y of cylinder in the scanner system
        x, y = distance*cos(bearing), distance*sin(bearing)
        result.append((np.array([distance, bearing]), np.array([x, y])))
    return result
