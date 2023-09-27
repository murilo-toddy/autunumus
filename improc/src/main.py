import logging
import time
import cv2
import proc
import shared.zmqprovider as zmq


logging.basicConfig(
        level=logging.INFO, 
        datefmt="%d-%m-%y %H:%M:%S",
        format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger()


if __name__ == "__main__":
    zmq_provider = zmq.ZMQProvider(
            path="localhost", 
            operation_mode=zmq.ZMQSocketOperation.PULL,
            port=5555
        )

    while True:
        start_time = time.time()

        if (frame := zmq_provider.receive_data()) is None:
            logger.info("Frame not received, waiting")
            continue

        cv2.imshow("Input", frame)
        
        c = cv2.waitKey(1)
        if c == 27:
            logger.info("Escape key pressed, stopping")
            # zmq_provider.close()
            quit()
        
        end_time = time.time()
        # logger.info(f"Frame acquisition took {round(end_time - start_time, 4):<6}")

        proc_start = time.time()
        proc.process_frame(frame)
        proc_end = time.time()
        # logger.info(f"Cycle took {round(proc_end - start_time, 4):<6} seconds")

