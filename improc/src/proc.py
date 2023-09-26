from collections.abc import MutableSet
import multiprocessing
import cv2

color_mask_dict = {
    "YELLOW": [
        [
            (16, 188, 116), 
            (63, 255, 238),
        ]
    ],
    "BLUE": [
        [
            (88, 134, 125),
            (132, 236, 200),
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

def process_frame(frame):
    count = 0
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    with multiprocessing.Pool() as pool:
    for color, masks in color_mask_dict.items():
        masked_images = []
        for mask in masks:
            masked_images.append(cv2.inRange(hsv_frame, mask[0], mask[1]))

        masked_image = masked_images[0].copy()

        for i in range(1, len(masked_images)):
            cv2.add(masked_image, masked_images[i])

        cv2.imshow(color, masked_image)
        blurred = cv2.GaussianBlur(masked_image, (5, 5), cv2.BORDER_DEFAULT)
        canny = cv2.Canny(blurred, 50, 150)

        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 2))
        dilated = cv2.dilate(canny, kernel)

        eroded = cv2.erode(dilated, kernel)

        contours, _ = cv2.findContours(
            eroded, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        
        for contour in contours:
            if cv2.contourArea(contour) < 10: 
                continue

            perimeter = cv2.arcLength(contour, True)
            poly_contour = cv2.approxPolyDP(contour, 0.02 * perimeter, True)

            if len(poly_contour) > 10 or len(poly_contour) < 3:
                continue

            convex_contour = cv2.convexHull(poly_contour)
            rect = cv2.boundingRect(convex_contour)
            height_center = rect[1] + (rect[3] // 2) 

            pts_above_center, pts_below_center = [], []
            for point in convex_contour:
                point = point[0]
                if point[1] < height_center:
                    pts_above_center.append(point)
                else:
                    pts_below_center.append(point)

            left_bound = right_bound = pts_below_center[0]
            for point in pts_below_center:
                if point[0] < left_bound[0]:
                    left_bound = point
                if point[0] > right_bound[0]:
                    right_bound = point

            aspect_ratio = float(rect[2]) / float(rect[3])
            if aspect_ratio > 0.8:
                continue

            if not all(left_bound[0] < point[0] < right_bound[0] for point in pts_above_center):
                continue

            higher_point = pts_above_center[0]
            for point in pts_above_center:
                if point[1] < higher_point[1]:
                    higher_point = point

            result = (higher_point, left_bound, right_bound)
            cv2.rectangle(frame, rect, (0, 255, 0))
            count += 1
    
    cv2.imshow("result", frame)
    print(f"found {count} cones")
