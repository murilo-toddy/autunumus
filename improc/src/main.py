import logging
import time
import zmq
import cv2

import proc


logging.basicConfig(
        level=logging.INFO, 
        datefmt="%d-%m-%y %H:%M:%S",
        format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger()


if __name__ == "__main__":
    context = zmq.Context()

    base_address = "tcp://localhost"
    frame_port = f"{base_address}:5555"

    frame_socket = context.socket(zmq.PULL)
    frame_socket.connect(frame_port)

    while True:
        start_time = time.time()
        frame = frame_socket.recv_pyobj()
        cv2.imshow("Input", frame)
        
        c = cv2.waitKey(1)
        if c == 27:
            logger.info("Escape key pressed, stopping")
            frame_socket.close()
            quit()
        
        end_time = time.time()
        # logger.info(f"Frame acquisition took {round(end_time - start_time, 4):<6}")

        proc_start = time.time()
        proc.process_frame(frame)
        proc_end = time.time()
        # logger.info(f"Cycle took {round(proc_end - start_time, 4):<6} seconds")

