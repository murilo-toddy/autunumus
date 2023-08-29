class Vehicle:
    """
    This class encapsulates all information related to the vehicle,
    such as mechanical constraints and movement model
    This should behave as a singleton and be injected into classes
    that depend on it
    """
    def __init__(
            self,
            scanner_displacement: float,
            vehicle_width: float
        ):
        pass

    def move(self):
        pass

