import zmq
from enum import Enum


class ZMQSocketOperation(Enum):
    PUSH = 1
    PULL = 2


class ZMQProvider:
    def __init__(
                self, 
                path: str,
                operation_mode: ZMQSocketOperation,
                port: int = 5555,
                pull_timeout_milliseconds: int = 1000,
            ):
        self.timeout_milliseconds = pull_timeout_milliseconds
        self.context = zmq.Context()

        base_address = f"tcp://{path}"
        address = f"{base_address}:{port}"

        if operation_mode == ZMQSocketOperation.PULL:
            self.__setup_pull_socket(address)
        else:
            self.__setup_push_socket(address)

    def __setup_pull_socket(self, address: str) -> None:
        self.socket = self.context.socket(zmq.PULL)
        self.socket.connect(address)
        self.poller = zmq.Poller()
        self.poller.register(self.socket, zmq.POLLIN)

    def __setup_push_socket(self, address: str) -> None:
        self.socket = self.context.socket(zmq.PUSH)
        self.socket.bind(address)

    def __socket_pop(self):
        event = self.poller.poll(self.timeout_milliseconds)
        return self.socket.recv_pyobj() if event else None

    def __socket_push(self, data):
        self.socket.send_pyobj(data)

    def receive_data(self):
        return self.__socket_pop()

    def send_data(self, data):
        self.__socket_push(data)

    def close(self):
        self.socket.close()

