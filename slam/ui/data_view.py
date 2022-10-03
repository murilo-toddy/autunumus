from math import sin, cos, ceil
from re import A
from tkinter import *
from tkinter import filedialog

from robot_lib import *

# Canvas definitions
canvas_size = (600, 400)
world_size = (40.0, 25.0)
slider_index = 0


class DrawableObject(object):
    def draw(self, at_step):
        pass

    def background_draw(self):
        pass

    @staticmethod
    def get_ellipse_points(center, main_axis_angle, radius1, radius2,
                           start_angle=0.0, end_angle=2 * pi):
        """Generate points of an ellipse, for drawing (y axis down)."""
        points = []
        ax = radius1 * cos(main_axis_angle)
        ay = radius1 * sin(main_axis_angle)
        bx = - radius2 * sin(main_axis_angle)
        by = radius2 * cos(main_axis_angle)
        N_full = 40  # Number of points on full ellipse.
        N = int(ceil((end_angle - start_angle) / (2 * pi) * N_full))
        N = max(N, 1)
        increment = (end_angle - start_angle) / N
        for i in range(N + 1):
            a = start_angle + i * increment
            c = cos(a)
            s = sin(a)
            x = c * ax + s * bx + center[0]
            y = - c * ay - s * by + center[1]
            points.append((x, y))
        return points


class Trajectory(DrawableObject):
    def __init__(self, points, canvas,
                 world_extents, canvas_extents,
                 standard_deviations=[],
                 point_size2=2,
                 background_color="gray", cursor_color="red",
                 position_stddev_color="green", theta_stddev_color="#ffc0c0"):
        self.points = points
        self.standard_deviations = standard_deviations
        self.canvas = canvas
        self.world_extents = world_extents
        self.canvas_extents = canvas_extents
        self.point_size2 = point_size2
        self.background_color = background_color
        self.cursor_color = cursor_color
        self.position_stddev_color = position_stddev_color
        self.theta_stddev_color = theta_stddev_color
        self.cursor_object = None
        self.cursor_object2 = None
        self.cursor_object3 = None
        self.cursor_object4 = None

    def background_draw(self):
        if self.points:
            p_xy_only = []
            for p in self.points:
                self.canvas.create_oval(
                    p[0] - self.point_size2, p[1] - self.point_size2,
                    p[0] + self.point_size2, p[1] + self.point_size2,
                    fill=self.background_color, outline="")
                p_xy_only.append(p[0:2])
            self.canvas.create_line(*p_xy_only, fill=self.background_color)

    def draw(self, at_step):
        if self.cursor_object:
            self.canvas.delete(self.cursor_object)
            self.cursor_object = None
            self.canvas.delete(self.cursor_object2)
            self.cursor_object2 = None
        if at_step < len(self.points):
            p = self.points[at_step]
            # Draw position (point).
            self.cursor_object = self.canvas.create_oval(
                p[0] - self.point_size2 - 1, p[1] - self.point_size2 - 1,
                p[0] + self.point_size2 + 1, p[1] + self.point_size2 + 1,
                fill=self.cursor_color, outline="")
            # Draw error ellipse.
            if at_step < len(self.standard_deviations):
                stddev = self.standard_deviations[at_step]
                # Note this assumes correct aspect ratio.
                factor = canvas_size[0] / world_size[0] * 20
                points = self.get_ellipse_points(p, stddev[0],
                                                 stddev[1] * factor, stddev[2] * factor)
                if self.cursor_object4:
                    self.canvas.delete(self.cursor_object4)
                self.cursor_object4 = self.canvas.create_line(
                    *points, fill=self.position_stddev_color)
            if len(p) > 2:
                # Draw heading standard deviation.
                if at_step < len(self.standard_deviations) and \
                        len(self.standard_deviations[0]) > 3:
                    angle = min(self.standard_deviations[at_step][3], pi)
                    points = self.get_ellipse_points(p, p[2], 30.0, 30.0,
                                                     -3 * angle, 3 * angle)
                    points = [p[0:2]] + points + [p[0:2]]
                    if self.cursor_object3:
                        self.canvas.delete(self.cursor_object3)
                    self.cursor_object3 = self.canvas.create_polygon(
                        *points, fill=self.theta_stddev_color)
                # Draw heading.
                self.cursor_object2 = self.canvas.create_line(p[0], p[1],
                                                              p[0] + cos(p[2]) * 50,
                                                              p[1] - sin(p[2]) * 50,
                                                              fill=self.cursor_color)


class Landmarks(DrawableObject):
    # In contrast other classes, Landmarks stores the original world coords and
    # transforms them when drawing.
    def __init__(self, landmarks, canvas, canvas_extents, world_extents, color="gray"):
        self.landmarks = landmarks
        self.canvas = canvas
        self.canvas_extents = canvas_extents
        self.world_extents = world_extents
        self.color = color

    def background_draw(self):
        for l in self.landmarks:
            if l[0] == 'C':
                x, y = l[1:3]
                ll = to_world_canvas((x - l[3], y - l[3]), self.canvas_extents, self.world_extents)
                ur = to_world_canvas((x + l[3], y + l[3]), self.canvas_extents, self.world_extents)
                self.canvas.create_oval(ll[0], ll[1], ur[0], ur[1], fill=self.color)

    def draw(self, at_step):
        # Landmarks are background only.
        pass


class Points(DrawableObject):
    # Points, optionally with error ellipses.
    def __init__(self, points, canvas, color="red", radius=5, ellipses=[], ellipse_factor=5.0):
        self.points = points
        self.canvas = canvas
        self.color = color
        self.radius = radius
        self.ellipses = ellipses
        self.ellipse_factor = ellipse_factor
        self.cursor_objects = []

    def background_draw(self):
        pass

    def draw(self, at_step):
        if self.cursor_objects:
            for o in self.cursor_objects:
                self.canvas.delete(o)
            self.cursor_objects = []
        if at_step < len(self.points):
            for i in range(len(self.points[at_step])):
                # Draw point.
                c = self.points[at_step][i]
                self.cursor_objects.append(self.canvas.create_oval(
                    c[0] - self.radius, c[1] - self.radius,
                    c[0] + self.radius, c[1] + self.radius,
                    fill=self.color))
                # Draw error ellipse if present.
                if at_step < len(self.ellipses) and i < len(self.ellipses[at_step]):
                    e = self.ellipses[at_step][i]
                    points = self.get_ellipse_points(c, e[0], e[1] * self.ellipse_factor,
                                                     e[2] * self.ellipse_factor)
                    self.cursor_objects.append(self.canvas.create_line(
                        *points, fill=self.color))


# Particles are like points but add a direction vector.
class Particles(DrawableObject):
    def __init__(self, particles, canvas, color="red", radius=1.0,
                 vector=8.0):
        self.particles = particles
        self.canvas = canvas
        self.color = color
        self.radius = radius
        self.vector = vector
        self.cursor_objects = []

    def background_draw(self):
        pass

    def draw(self, at_step):
        if self.cursor_objects:
            for o in self.cursor_objects:
                self.canvas.delete(o)
            self.cursor_objects = []
        if at_step < len(self.particles):
            for c in self.particles[at_step]:
                self.cursor_objects.append(self.canvas.create_oval(
                    c[0] - self.radius, c[1] - self.radius,
                    c[0] + self.radius, c[1] + self.radius,
                    fill=self.color, outline=self.color))
                self.cursor_objects.append(self.canvas.create_line(
                    c[0], c[1],
                    c[0] + cos(c[2]) * self.vector,
                    c[1] - sin(c[2]) * self.vector,
                    fill=self.color))


# World canvas is x right, y up, and scaling according to canvas/world extents.
def to_world_canvas(world_point, canvas_extents, world_extents):
    """Transforms a point from world coord system to world canvas coord system."""
    x = int(world_point[0] / world_extents[0] * canvas_extents[0])
    y = int(canvas_extents[1] - 1 - world_point[1] / world_extents[1] * canvas_extents[1])
    return x, y


# Sensor canvas is "in driving direction", with x up, y left, (0,0) in the center
# and scaling according to canvas_extents and max_scanner_range.
def to_sensor_canvas(sensor_point, canvas_extents, scanner_range):
    """Transforms a point from sensor coordinates to sensor canvas coord system."""
    scale = canvas_extents[0] / 2.0 / scanner_range
    x = int(canvas_extents[0] / 2.0 - sensor_point[1] * scale)
    y = int(canvas_extents[1] / 2.0 - 1 - sensor_point[0] * scale)
    return x, y


def slider_moved(index):
    """Callback for moving the scale slider."""
    global slider_index
    slider_index = int(index)
    # Call all draw objects.
    for d in draw_objects:
        d.draw(slider_index)

    # Print info about current point.
    info.config(text=robot.info(slider_index))


def add_file():
    filename = filedialog.askopenfilename(filetypes=[("all files", ".*"), ("txt files", ".txt")])
    if filename:
        # If the file is in the list already, remove it (so it will be appended
        # at the end).
        if filename in all_file_names:
            all_file_names.remove(filename)
        all_file_names.append(filename)
        load_data()


def load_data():
    global canvas_size, world_size
    for filename in all_file_names:
        robot.read(filename)

    global draw_objects
    draw_objects = []
    scale.configure(to=robot.size() - 1)

    # Insert: landmarks.
    draw_objects.append(Landmarks(robot.landmarks, world_canvas, canvas_size, world_size))

    # Insert: reference trajectory.
    positions = [to_world_canvas(pos, canvas_size, world_size) for pos in robot.reference_positions]
    draw_objects.append(Trajectory(positions, world_canvas, world_size, canvas_size,
                                   cursor_color="red", background_color="#FFB4B4"))

    # Insert: world objects, landmarks and corresponding world objects, ellipses.
    if robot.world_landmarks:
        positions = [[to_world_canvas(pos, canvas_size, world_size)
                      for pos in landmarks_one_scan]
                     for landmarks_one_scan in robot.world_landmarks]
        # Also setup landmarks if present.
        # Note this assumes correct aspect ratio.
        factor = canvas_size[0] / world_size[0]
        draw_objects.append(Points(positions, world_canvas, "#DC23C5",
                                   ellipses=robot.world_ellipses,
                                   ellipse_factor=factor))

    # Insert: detected landmarks, transformed into world coord system.
    if robot.detected_landmarks and robot.filtered_positions and \
            len(robot.filtered_positions[0]) > 2:
        positions = []
        for i in range(min(len(robot.detected_landmarks), len(robot.filtered_positions))):
            this_pose_positions = []
            pos = robot.filtered_positions[i]
            dx = cos(pos[2])
            dy = sin(pos[2])
            for pole in robot.detected_landmarks[i]:
                x = pole[0] * dx - pole[1] * dy + pos[0]
                y = pole[0] * dy + pole[1] * dx + pos[1]
                p = to_world_canvas((x, y), canvas_size, world_size)
                this_pose_positions.append(p)
            positions.append(this_pose_positions)
        draw_objects.append(Points(positions, world_canvas, "#88FF88"))

    # Insert: particles.
    if robot.particles:
        positions = [
            [(to_world_canvas(pos, canvas_size, world_size) + (pos[2],))
             for pos in particles_one_scan]
            for particles_one_scan in robot.particles]
        draw_objects.append(Particles(positions, world_canvas, "#80E080"))

    # Insert: filtered trajectory.
    if robot.filtered_positions:
        if len(robot.filtered_positions[0]) > 2:
            positions = [tuple(list(to_world_canvas(pos, canvas_size, world_size)) + [pos[2]]) for pos in
                         robot.filtered_positions]
        else:
            positions = [to_world_canvas(pos, canvas_size, world_size) for pos in robot.filtered_positions]
        # If there is error ellipses, insert them as well.
        draw_objects.append(Trajectory(positions, world_canvas, world_size, canvas_size,
                                       standard_deviations=robot.filtered_stddev,
                                       cursor_color="blue", background_color="lightblue",
                                       position_stddev_color="#8080ff", theta_stddev_color="#c0c0ff"))

    # Start new canvas and do all background drawing.
    world_canvas.delete(ALL)
    for d in draw_objects:
        d.background_draw()


def autorun():
    global slider_index
    if auto_run_info.get() == 1:
        slider_index = (slider_index + 1) % (robot.size() - 1)
        slider_moved(slider_index)
        root.after(5, autorun)


# Main program.
if __name__ == '__main__':

    # Load all robot data
    robot = Robot()

    # Graphics Interface setup
    root = Tk()
    frame1 = Frame(root)
    frame1.pack()
    world_canvas = Canvas(frame1, width=canvas_size[0], height=canvas_size[1], bg="white")
    world_canvas.pack(side=LEFT)
    scale = Scale(root, orient=HORIZONTAL, command=slider_moved)
    scale.pack(fill=X)
    info = Label(root)
    info.pack()
    frame2 = Frame(root)
    frame2.pack()
    load = Button(frame2, text="Load data file", command=add_file)
    load.pack(side=LEFT)
    auto_run_info = IntVar()
    auto_run = Checkbutton(frame2, text="Autorun", variable=auto_run_info, command=autorun)
    auto_run.pack(side=RIGHT)
    reload_all = Button(frame2, text="Reload files", command=load_data)
    reload_all.pack(side=RIGHT)

    # The list of objects to draw.
    draw_objects = []

    # Ask for file.
    all_file_names = []
    add_file()

    root.mainloop()
    root.destroy()

