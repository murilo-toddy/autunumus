import zmq


if __name__ == "__main__":
    context = zmq.Context()

    base_address = "tcp://127.0.0.1"
    vehicle_port = f"{base_address}:5555"
    scanner_port = f"{base_address}:5556"

    vehicle_socket = context.socket(zmq.PULL)
    vehicle_socket.connect(vehicle_port)

    scanner_socket = context.socket(zmq.PULL)
    scanner_socket.connect(scanner_port)

    while True:
        vehicle_data = vehicle_socket.recv_pyobj()
        scanner_data = scanner_socket.recv_pyobj()
        print("Vehicle data received:", vehicle_data)
        print("Scanner data received:", scanner_data)

