import zmq
import numpy as np


class ZMQProvider:
    def __init__(self):
        self.context = zmq.Context()
        # TODO: this address should ideally be updated without needing
        # to re-build the container
        base_address = "tcp://slam_input_container"

        vehicle_address = f"{base_address}:5555"
        scanner_address = f"{base_address}:5556"

        self.vehicle_socket = self.__setup_pull_socket(vehicle_address)
        self.scanner_socket = self.__setup_pull_socket(scanner_address)

    def __setup_pull_socket(self, address: str):
        socket = self.context.socket(zmq.PULL)
        socket.connect(address)
        return socket

    # TODO: is this compatible with other languages?
    def __socket_pop(self, socket: zmq.Socket):
        return socket.recv_pyobj()

    def read_vehicle_data(self) -> list[float]:
        return self.__socket_pop(self.vehicle_socket)

    def read_scanner_data(self) -> np.ndarray:
        return self.__socket_pop(self.scanner_socket)

    def read_data(self) -> tuple[list[float], np.ndarray]:
        return self.read_vehicle_data(), self.read_scanner_data()

