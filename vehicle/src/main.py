import time
import configparser
import logging

from webcamprovider import WebcamProvider
from zmqprovider import ZMQProvider


logging.basicConfig(
    level=logging.INFO, 
    datefmt="%d-%m-%y %H:%M:%S",
    format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger(__name__)


if __name__ == "__main__":
    config = configparser.ConfigParser()
    config.read("config.ini")

    logger.info("Starting application")

    width, height = config.getint("resolution", "width"), config.getint("resolution", "height")
    webcam_provider = WebcamProvider(width, height)

    host_container_name = "*"
    zmq_provider = ZMQProvider(host_container_name)

    while True:
        start_time = time.time()
        frame = webcam_provider.get_frame()
        end_time = time.time()
        logger.info(f"Frame read took {round(end_time - start_time, 4):<6} seconds")
        zmq_provider.send_frame(frame)
    
