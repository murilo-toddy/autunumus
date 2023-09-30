import numpy as np
import zmq
import logging

logging.basicConfig(
        level=logging.INFO, 
        datefmt="%d-%m-%y %H:%M:%S",
        format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger()

minimum_valid_distance = 1
depth_jump = 4
landmark_offset = 0.2


class Lidar:
    @staticmethod
    def beam_index_to_angle(index: float) -> float:
        """ Convert a beam index to an angle, in radians """ 
        return -1.5707999467849731 + index * 0.004369401838630438

    @staticmethod
    def min_max_bearing() -> tuple[float, float]:
        """ Minimum and maximum angle detected by lidar """
        return -1.5707999467849731, 1.5707999467849731


def compute_derivative(scanner_data: list[float], min_dist: float) -> list[float]:
    derivative_list = [0.0]
    for i in range(1, len(scanner_data) - 1):
        left = scanner_data[i-1]
        right = scanner_data[i+1]
        if left > min_dist and right > min_dist:
            derivative = (right - left) / 2.0
            derivative_list.append(derivative)
        else:
            derivative_list.append(0)
    derivative_list.append(0)
    return derivative_list


def find_landmarks(
            scan: list[float], 
            scan_derivative: list[float], 
            jump: float, 
            min_dist: float,
        ) -> list[tuple[float, float]]:
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


def get_landmarks_from_scan(
            scan: list[float], 
            jump: float, 
            min_dist: float, 
            landmark_offset: float,
        ) -> list[tuple[np.ndarray, np.ndarray]]:
    der = compute_derivative(scan, min_dist)
    landmarks = find_landmarks(scan, der, jump, min_dist)
    result = []
    for c in landmarks:
        # Compute the angle and distance measurements
        bearing = Lidar.beam_index_to_angle(c[0])
        distance = c[1] + landmark_offset
        # Compute x, y of landmark in the scanner system
        x, y = distance * np.cos(bearing), distance * np.sin(bearing)
        result.append((np.array([distance, bearing]), np.array([x, y])))
    return result


def read_robot_data_from_files(
            vehicle_status_filename: str = "./data/motor.txt",
            scanner_filename: str = "./data/lidar.txt",
        ) -> list[tuple[list[float], list[float]]]:
    with open(vehicle_status_filename) as v, open(scanner_filename) as s:
        robot_data = []
        for vehicle_sample_raw, scanner_sample_raw in zip(v, s):
            vehicle_data = [float(s) for s in vehicle_sample_raw.split()[1:]]
            scanner_data = tuple([float(s) for s in scanner_sample_raw.split()[3:]])
            robot_data.append((
                vehicle_data,
                scanner_data
            ))
        return robot_data


def process_robot_data(robot_data: list[tuple[list[float], list[float]]]):
    control_data = []
    landmarks = []
    for vehicle_sample, scanner_sample in robot_data:
        control_data.append(vehicle_sample)
        landmarks.append(
            get_landmarks_from_scan(
                scanner_sample,
                depth_jump,
                minimum_valid_distance,
                landmark_offset,
            )
        )
    return list(zip(control_data, landmarks))


if __name__ == "__main__":
    context = zmq.Context()
    base_address = "tcp://*"
    vehicle_port = f"{base_address}:5555"
    scanner_port = f"{base_address}:5556"

    vehicle_socket = context.socket(zmq.PUSH)
    vehicle_socket.bind(vehicle_port)

    scanner_socket = context.socket(zmq.PUSH)
    scanner_socket.bind(scanner_port)

    robot_data = process_robot_data(read_robot_data_from_files())
    for i, (vehicle_data, scanner_data) in enumerate(robot_data):
        logger.info(f"Sending vehicle and landmarks ({i + 1}/{len(robot_data)})")
        logger.info(f"Vehicle data: {vehicle_data}")
        logger.info(f"Landmark data: {scanner_data}")
        vehicle_socket.send_pyobj(vehicle_data)
        scanner_socket.send_pyobj(scanner_data)

    vehicle_socket.close()
    scanner_socket.close()

