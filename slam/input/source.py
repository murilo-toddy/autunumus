import zqm


def process_robot_data(robot_data: list[tuple[str, str]]):
    pass


def read_robot_data_from_files(
            vehicle_status_filename: str = "./data/motor.txt",
            landmark_filename: str = "./data/lidar.txt"
        ) -> list[tuple[str, str]]:
    with open(vehicle_status_filename) as v, open(landmark_filename) as l:
        return list(zip(v, l))


if __name__ == "__main__":
    ctx = zmq.Context()
    socket = ctx.socket(zmq.PUB)
    socket.bind("tcp://*:5555")  # robot telemetry
    socket.bind("tcp://*:5556")  # landmarks

    robot_data = process_robot_data(read_robot_data_from_files())
    for vehicle, landmark in robot_data:
        socket.send(b"{}".format(vehicle))

