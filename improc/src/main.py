import logging
import time
import cv2
import shared.zmqprovider as zmq
import preprocessing
import mask
import geometry

logger = logging.getLogger(__name__)

color_mask_dict = {
    "YELLOW": [
        [
            ( 16, 188, 116), 
            ( 63, 255, 238),
        ]
    ],
    "BLUE": [
        [
            ( 82, 137, 147),
            (120, 255, 255),
        ]
    ],
    "RED": [
        [
            (  0, 120,  80),
            ( 15, 255, 255),
        ],
        [
            (159, 135,  90),
            (179, 255, 160),
        ],
    ],
}

CONE_HEIGHT_CONSTANT = 75 * 421.8

logging.basicConfig(
        level=logging.INFO, 
        datefmt="%d-%m-%y %H:%M:%S",
        format="[%(asctime)s - %(levelname)s] %(module)s: %(message)s", 
)

logger = logging.getLogger()


def estimate_distance(height):
    return CONE_HEIGHT_CONSTANT / height


def process_frame(frame):
    count = 0
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    for color, masks in color_mask_dict.items():
        masked_image = mask.mask_frame(hsv_frame, masks)

        cv2.imshow(color, masked_image)
        contours = preprocessing.get_contours(masked_image)
        
        for contour in contours:
            rect = geometry.contour_processing(contour)
            if rect is not None:
                cv2.rectangle(frame, rect, (0, 255, 0))
                cv2.putText(
                        frame, 
                        f"{color} LANDMARK @ {estimate_distance(rect[3])}",
                        (rect[0], rect[1]),
                        cv2.FONT_HERSHEY_DUPLEX, 0.7, (255, 255, 255), 1
                    )
                count += 1
    
    cv2.imshow("result", frame)
    logger.info(f"found {count} cones")

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
            zmq_provider.close()
            quit()
        
        end_time = time.time()
        logger.info(f"Frame acquisition took {round(end_time - start_time, 4):<6}")

        proc_start = time.time()
        process_frame(frame)
        proc_end = time.time()
        logger.info(f"Cycle took {round(proc_end - start_time, 4):<6} seconds")

