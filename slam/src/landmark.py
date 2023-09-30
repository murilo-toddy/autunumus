from dataclasses import dataclass
import numpy as np


@dataclass
class Landmark:
    counter: int
    position: np.ndarray
    covariance: np.ndarray
#     def __init__(self, counter: int, position: np.ndarray, covariance: np.ndarray):
#         self.counter = counter
#         self.position = position
#         self.covariance = covariance

