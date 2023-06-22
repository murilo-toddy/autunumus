class Robot:
    def __init__(self):
        self.scan_data = []
        self.motor_ticks = []

    def read(self, filename: str):
        with open(filename) as f:
            for line in f:
                line_data = line.split()
                if line_data[0] == 'S':
                    """ S - Scan data 
                        File format:
                        S n_measurements [measurements] """
                    self.scan_data.append(tuple([float(s) for s in line_data[3:]]))

                elif line_data[0] == 'M':
                    """ M - Motor control
                        File format:
                        M left_encoder right_encoder """
                    ticks = (float(line_data[1]), float(line_data[2]))
                    self.motor_ticks.append(ticks)

