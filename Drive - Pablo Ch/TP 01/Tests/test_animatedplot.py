# Import packages
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

# Fermi-Dirac Distribution
def fermi(E: float, E_f: float, T: float) -> float:
    k_b = 8.617 * (10**-5) # eV/K
    return 1/(np.exp((E - E_f)/(k_b * T)) + 1)

# Temperature values
T = np.linspace(100, 1000, 10)

# Get colors from coolwarm colormap
colors = plt.get_cmap('coolwarm', 10)

# Create figure and add axes
fig = plt.figure(figsize=(6, 4))
ax = fig.add_subplot(111)

# Import animation package
from matplotlib.animation import FuncAnimation
# Create variable reference to plot
f_d, = ax.plot([], [], linewidth=2.5)
# Add text annotation and create variable reference
temp = ax.text(1, 1, '', ha='right', va='top', fontsize=24)

# Animation function
def animate(i):
    x = np.linspace(0, 1, 100)
    y = fermi(x, 0.5, T[i])
    f_d.set_data(x, y)
    f_d.set_color(colors(i))
    temp.set_text(str(int(T[i])) + ' K')
    temp.set_color(colors(i))

# Create animation
ani = FuncAnimation(fig=fig, func=animate, frames=range(len(T)), interval=500, repeat=True)

# Ensure the entire plot is visible
fig.tight_layout()
# Save and show animation
ani.save('AnimatedPlot.gif', writer='imagemagick', fps=2)