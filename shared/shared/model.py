from dataclasses import dataclass

@dataclass
class Landmark:
    spec: str
    distance: float
    angle: float
    width: float


@dataclass
class Vehicle:
    left_wheel_disp: float
    right_wheel_disp: float

