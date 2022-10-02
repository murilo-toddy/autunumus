from dataclasses import dataclass
from math import sqrt, sin, cos, atan2, pi

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


def get_mean(particles):
    """Compute mean position and heading from a given set of particles."""
    # Note this function would more likely be a part of FastSLAM or a base class
    # of FastSLAM. It has been moved here for the purpose of keeping the
    # FastSLAM class short in this tutorial.
    mean_x, mean_y = 0.0, 0.0
    head_x, head_y = 0.0, 0.0
    for p in particles:
        x, y, theta = p.pose
        mean_x += x
        mean_y += y
        head_x += cos(theta)
        head_y += sin(theta)
    n = max(1, len(particles))
    return np.array([mean_x / n, mean_y / n, atan2(head_y, head_x)])


def get_error_ellipse_and_heading_variance(particles, mean):
    """Given a set of particles and their mean (computed by get_mean()),
       returns a tuple: (angle, stddev1, stddev2, heading-stddev) which is
       the orientation of the xy error ellipse, the half axis 1, half axis 2,
       and the standard deviation of the heading."""
    # Note this function would more likely be a part of FastSLAM or a base class
    # of FastSLAM. It has been moved here for the purpose of keeping the
    # FastSLAM class short in this tutorial.
    center_x, center_y, center_heading = mean
    n = len(particles)
    if n < 2:
        return 0.0, 0.0, 0.0, 0.0

    # Compute covariance matrix in xy.
    sxx, sxy, syy = 0.0, 0.0, 0.0
    for p in particles:
        x, y, theta = p.pose
        dx = x - center_x
        dy = y - center_y
        sxx += dx * dx
        sxy += dx * dy
        syy += dy * dy
    cov_xy = np.array([[sxx, sxy], [sxy, syy]]) / (n-1)

    # Get variance of heading.
    var_heading = 0.0
    for p in particles:
        dh = (p.pose[2] - center_heading + pi) % (2*pi) - pi
        var_heading += dh * dh
    var_heading = var_heading / (n-1)

    # Convert xy to error ellipse.
    eigenvals, eigenvects = np.linalg.eig(cov_xy)
    ellipse_angle = atan2(eigenvects[1, 0], eigenvects[0, 0])

    return (ellipse_angle, sqrt(abs(eigenvals[0])),
            sqrt(abs(eigenvals[1])),
            sqrt(var_heading))