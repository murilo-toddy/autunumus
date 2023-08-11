import zmq
import logging

logging.basicConfig(
        level=logging.INFO, 
        datefmt="%d-%m-%y %H:%M:%S",
        format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger()


if __name__ == "__main__":
    context = zmq.Context()

    base_address = "tcp://zmq_source_container"
    vehicle_port = f"{base_address}:5555"
    scanner_port = f"{base_address}:5556"

    vehicle_socket = context.socket(zmq.PULL)
    vehicle_socket.connect(vehicle_port)

    scanner_socket = context.socket(zmq.PULL)
    scanner_socket.connect(scanner_port)

    while True:
        vehicle_data = vehicle_socket.recv_pyobj()
        scanner_data = scanner_socket.recv_pyobj()
        logger.info(f"Vehicle data received: {vehicle_data}")
        logger.info(f"Scanner data received: {scanner_data}")

