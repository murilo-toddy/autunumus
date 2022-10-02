from math import sin, cos

import numpy as np


# Convert robot pose (x, y, theta) into (x, y) in world coordinates
def scanner_to_world(pose, point: list[np.ndarray]) -> tuple[float, float]:
    dx = cos(pose[2])
    dy = sin(pose[2])
    x, y = point
    return x * dx - y * dy + pose[0], x * dy + y * dx + pose[1]


class Lidar:
    @staticmethod
    def beam_index_to_angle(index: float) -> float:
        """ Convert a beam index to an angle, in radians """ 
        return -1.5707999467849731 + index * 0.004369401838630438

    @staticmethod
    def min_max_bearing() -> tuple[float, float]:
        """ Minimum and maximum angle detected by lidar """
        return -1.5707999467849731, 1.5707999467849731
