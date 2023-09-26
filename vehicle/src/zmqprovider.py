import zmq


class ZMQProvider:
    def __init__(
            self, 
            host_container_name: str,
            frame_port: int = 5555,
            timeout_milliseconds: int = 1000,
        ):
        self.timeout_milliseconds = timeout_milliseconds
        self.context = zmq.Context()
        base_address = f"tcp://{host_container_name}"

        frame_address = f"{base_address}:{frame_port}"
        self.frame_socket = self.__setup_push_socket(frame_address)

    def __setup_push_socket(self, address: str) -> zmq.Socket:
        socket = self.context.socket(zmq.PUSH)
        socket.bind(address)
        return socket

    def __socket_push(self, socket: zmq.Socket, frame):
        return socket.send_pyobj(frame)

    def send_frame(self, frame) -> None:
        self.__socket_push(self.frame_socket, frame)

