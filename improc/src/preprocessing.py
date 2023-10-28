import cv2

def get_contours(frame):
    blurred = cv2.GaussianBlur(frame, (5, 5), cv2.BORDER_DEFAULT)
    canny = cv2.Canny(blurred, 50, 150)

    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 2))
    dilated = cv2.dilate(canny, kernel)

    eroded = cv2.erode(dilated, kernel)

    contours, _ = cv2.findContours(
        eroded, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
    )
    return contours

