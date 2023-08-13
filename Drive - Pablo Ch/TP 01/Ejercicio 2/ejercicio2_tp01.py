'''
Ejercicio 2

Aplicación del método de Euler a las ec. de movimiento del oscilador armónico con condiciones iniciales dadas.

'''
import numpy as np
import matplotlib.pyplot as plt

#El objetivo será determinar el valor x(j+1) y p(j+1) a partir de x(j) y p(j).
def paso(x_j, p_j, h):
	#Recibe los valores x_j y p_j y devuelve x_j+1 y p_j+1
	x_sig = x_j + h*p_j
	p_sig = p_j - h*x_j
	return x_sig, p_sig

def evolution(x,p, x0, p0, h):
	#Inicializo los vectores:
	x[0] = x0
	p[0] = p0

	#Aplico el paso:
	for j in range(cant_pasos - 1):
		x[j+1], p[j+1] = paso(x[j], p[j], h)
	return x, p


#Condiciones iniciales
x0 = 1
p0 = 1


hs = [0.1, 0.5, 1]

for h in hs:
	#Defino los vectores posición y momento
	t_i = 0; t_f = 31
	cant_pasos = np.int((t_f-t_i)/h)
	t = np.linspace(t_i, t_f, cant_pasos)
	x = np.zeros(cant_pasos)
	p = np.zeros(cant_pasos)

	x, p = evolution(x,p,x0,p0,h)
	plt.plot(x,p, label = 'h')


plt.show()