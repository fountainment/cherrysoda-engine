import numpy as np
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def get_spin(radius_scale, height_scale, rounds):
    xs, ys, zs = [], [], []
    theta = 0.0
    delta = 0.1
    twopi = math.pi * 2.0
    for i in range(int(rounds * twopi / delta)):
        theta += delta
        radius = theta / twopi * radius_scale
        x = np.cos(theta) * radius
        y = np.sin(theta) * radius
        xs.append(x)
        ys.append(y)
        zs.append(theta / twopi * height_scale)
    return xs, ys, zs

def main():
    fig = plt.figure()
    ax = Axes3D(fig)

    ax.plot(*get_spin(1.0,  3.0,  5.0))
    ax.plot(*get_spin(1.05, 3.15, 5.0))
    plt.show()

if __name__ == '__main__':
    main()
