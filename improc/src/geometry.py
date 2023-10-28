import cv2

MINIMUM_AREA_THRESHOLD = 10
MIN_EDGES = 3
MAX_EDGES = 10


def contour_processing(contour):
    if cv2.contourArea(contour) < MINIMUM_AREA_THRESHOLD:
        return None

    perimeter = cv2.arcLength(contour, True)
    poly_contour = cv2.approxPolyDP(contour, 0.02 * perimeter, True)

    if not (MIN_EDGES < len(poly_contour) < MAX_EDGES):
        return None

    convex_contour = cv2.convexHull(poly_contour)
    rect = cv2.boundingRect(convex_contour)

    aspect_ratio = float(rect[2]) / float(rect[3])
    if aspect_ratio > 0.8:
        return None

    height_center = rect[1] + (rect[3] // 2) 

    pts_above_center = [p[0] for p in convex_contour if p[0][1] < height_center]
    pts_below_center = [p[0] for p in convex_contour if p[0][1] >= height_center]

    left_bound = min(pts_below_center, key=lambda p: p[0])
    right_bound = max(pts_below_center, key=lambda p: p[0])

    if not all(left_bound[0] < p[0] < right_bound[0] for p in pts_above_center):
        return None

    higher_point = min(pts_above_center, key=lambda p: p[1])

    result = (higher_point, left_bound, right_bound)
    return rect

