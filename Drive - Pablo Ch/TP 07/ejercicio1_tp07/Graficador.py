'''
Graficador de las posiciones de las partículas
'''

#Importo los datos:
import numpy as np
from matplotlib import pyplot as plt

posiciones_x = np.loadtxt("mesh_position_x.dat")
posiciones_y = np.loadtxt("mesh_position_y.dat")

t = 0 #tiempo que desea graficar
N = 4**2;
plt.scatter(posiciones_x[t,:], posiciones_y[t,:])
plt.show()





