import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def get_spin(s, a, b, c):
    xs, ys, zs = [], [], []
    theta = s
    for i in range(50):
        delta = 0.5
        t = theta + a
        if t > 0:
            delta = np.clip(1.0 / t, 0.25, delta)
        theta += delta
        t = theta
        if t < 0:
            t = 0
        x = np.cos(t + a) * t * b
        y = np.sin(t + a) * t * b
        xs.append(x)
        ys.append(y)
        zs.append(t * c)
    return xs, ys, zs

def main():
    fig = plt.figure()
    ax = Axes3D(fig)

    ax.plot(*get_spin(0.0, 0.0, 1.0, 3.0))
    ax.plot(*get_spin(-5.28, 5.28, 1.0, 3.0))
    # ax.plot(*get_spin(-6.28, 0.0, 0.96, 3.0))
    plt.show()

if __name__ == '__main__':
    main()