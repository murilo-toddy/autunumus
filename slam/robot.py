class Robot:
    def __init__(self):
        self.scan_data = []
        self.motor_ticks = []

    def read(self, filename):
        with open(filename) as f:
            for l in f:
                sp = l.split()
                # S is the scan data.
                # File format:
                #  S timestamp[in ms] distances[in mm] ...
                # Or, in previous versions (set s_record_has_count to True):
                #  S timestamp[in ms] count distances[in mm] ...
                # Stored: A list of tuples [ [(scan1_distance,... ), (scan2_distance,...) ]
                #   containing all scans, in scan_data.
                if sp[0] == 'S':
                    self.scan_data.append(tuple([float(s) for s in sp[3:]]))

                # M is the motor data.
                # File format: M timestamp[in ms] pos[in ticks] tachoCount[in ticks] acceleration[deg/s^2] rotationSpeed[deg/s] ...
                #   (4 values each for: left motor, right motor, and third motor (not used)).
                # Stored: A list of tuples [ (inc-left, inc-right), ... ] with tick increments, in motor_ticks.
                # Note that the file contains absolute ticks, but motor_ticks contains the increments (differences).
                elif sp[0] == 'M':
                    ticks = (float(sp[1]), float(sp[2]))
                    self.motor_ticks.append(ticks)
