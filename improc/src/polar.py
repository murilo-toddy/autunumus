import tkinter as tk
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

data = [
    (527.25, 6.819609375000001 - 23.7),
    (329.53, 22.13578125 - 23.7),
    (632.7, 38.085468750000004 - 23.7),
]

def create_polar_plot():
    fig = plt.figure()
    ax = fig.add_subplot(111, polar=True)

    distances, angles = zip(*data)
    angles = np.deg2rad(angles)  # Convert angles to radians

    ax.scatter(angles, distances, c='red', marker='o', s=100)
    ax.set_theta_zero_location('N')
    ax.set_theta_direction(-1)
    ax.set_rlabel_position(90)
    ax.set_thetamin(-30)  # Set the minimum angle
    ax.set_thetamax(30)   # Set the maximum angle
    ax.set_rmax(800)

    for i in range(len(data)):
        ax.annotate(f'({data[i][0]}, {round(data[i][1], 2)})', (angles[i], distances[i] + 2),
                    ha='center', va='bottom')

    canvas = FigureCanvasTkAgg(fig, master=window)
    canvas.get_tk_widget().pack()
    canvas.draw()

window = tk.Tk()
window.title("Polar Plot")

create_polar_plot()
window.mainloop()
