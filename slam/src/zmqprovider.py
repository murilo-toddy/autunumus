import shared.zmqprovider as zmq
import numpy as np


class ZMQProvider:
    def __init__(
                self, 
                path: str,
                vehicle_port: int = 5555,
                landmark_port: int = 5556,
                timeout_milliseconds: int = 1000,
            ):
        self.timeout_milliseconds = timeout_milliseconds
        self.vehicle_zmq_provider = zmq.ZMQProvider(
                path=path,
                operation_mode=zmq.ZMQSocketOperation.PULL,
                port=vehicle_port,
                pull_timeout_milliseconds=timeout_milliseconds,
            )
        self.landmark_zmq_provider = zmq.ZMQProvider(
                path=path,
                operation_mode=zmq.ZMQSocketOperation.PULL,
                port=landmark_port,
                pull_timeout_milliseconds=timeout_milliseconds,
            )

    def read_vehicle_data(self) -> list[float] | None:
        return self.vehicle_zmq_provider.receive_data()

    def read_landmark_data(self) -> np.ndarray | None:
        return self.landmark_zmq_provider.receive_data()

    def read_data(self) -> tuple[list[float] | None, np.ndarray | None]:
        return self.read_vehicle_data(), self.read_landmark_data()

