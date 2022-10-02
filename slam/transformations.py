from math import sin, cos


def scanner_to_world(pose, point):
    """Given a robot pose (rx, ry, heading) and a point (x, y) in the
        scanner's coordinate system, return the point's coordinates in the
        world coordinate system."""
    dx = cos(pose[2])
    dy = sin(pose[2])
    x, y = point
    return (x * dx - y * dy + pose[0], x * dy + y * dx + pose[1])   


class Lidar:
    @staticmethod
    def beam_index_to_angle(i, mounting_angle=0.0):
        """Convert a beam index to an angle, in radians.""" 
        return -1.5707999467849731 + i * 0.004369401838630438

    @staticmethod
    def min_max_bearing():
        return (-1.5707999467849731, 1.5707999467849731)