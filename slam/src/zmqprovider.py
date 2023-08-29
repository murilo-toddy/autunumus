import zmq
import numpy as np


class ZMQProvider:
    def __init__(self, timeout_seconds: int = 1000):
        self.timeout_seconds = timeout_seconds
        self.context = zmq.Context()
        # TODO: this address should ideally be updated without needing
        # to re-build the container
        base_address = "tcp://slam_input_container"

        vehicle_address = f"{base_address}:5555"
        scanner_address = f"{base_address}:5556"

        self.vehicle_handler = self.__setup_pull_socket(vehicle_address)
        self.scanner_handler = self.__setup_pull_socket(scanner_address)

    def __setup_pull_socket(self, address: str) -> tuple[zmq.Socket, zmq.Poller]:
        socket = self.context.socket(zmq.PULL)
        socket.connect(address)
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        return socket, poller

    # TODO: is this compatible with other languages?
    def __socket_pop(self, handler: tuple[zmq.Socket, zmq.Poller]):
        socket, poller = handler
        event = poller.poll(self.timeout_seconds)
        if socket in event:
            return socket.recv_pyobj()
        return None

    def read_vehicle_data(self) -> list[float] | None:
        return self.__socket_pop(self.vehicle_handler)

    def read_scanner_data(self) -> np.ndarray | None:
        return self.__socket_pop(self.scanner_handler)

    def read_data(self) -> tuple[list[float] | None, np.ndarray | None]:
        return self.read_vehicle_data(), self.read_scanner_data()

