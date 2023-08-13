'''
Ejercicio 1

Se busca calcular sin(x) en x = pi/6 usando las fórmulas de diferencias finitas a o(h) (adelantada), o(h^2) (centrada) y o(h^4) (tmb centrada)
'''

import numpy as np
import matplotlib.pyplot as plt

#Python no define nativamente precisión simple y doble, así que las defino usando la siguiente función que aprovecha una funcionalidad de numpy:

def round(z, precision):
	if precision == 32:
		return np.float32(z)
	elif precision == 64:
		return np.float64(z)


#Defino las fórmulas de diferencias finitas con su orden como nombre:


def o_h(x,h,precision):
	approximation = (np.sin(x + h) - np.sin(x))/h
	return round(approximation, precision)

def o_h2(x,h, precision):
	approximation = (np.sin(x+h) - np.sin(x-h))/(2*h)
	return round(approximation, precision)

def o_h4(x,h, precision):
	approximation = (-np.sin(x+2*h) + 8*np.sin(x+h) - 8*np.sin(x-h) + np.sin(x-2*h) )/(12*h)
	return round(approximation, precision)

#Defino el error:

def error(derivate_aprox,x,h, precision):
	#derivate_aprox es la fórmula de diferencia finita como función
	return np.abs(np.float64(derivate_aprox(x,h,precision)) - np.cos(x))




#Grafico el error en función de h para las 3 fórmulas en precisión simple y doble en escala logarítmica
x0 = np.pi/6

inic = 0
final = 10**(-1)
cant_pasos = 1000
paso = (final - inic)/cant_pasos
h = np.linspace(inic + paso,final + paso,cant_pasos)
print(h[0])
plt.plot(np.log(h), np.log(error(o_h, x0, h, 64)), c = 'r', label = 'o(h) precision doble')
plt.scatter(np.log(h), np.log(error(o_h, x0, h, 32)), c = 'r', label = 'o(h) precision simple')
plt.plot(np.log(h), np.log(error(o_h2, x0, h, 64)), c = 'b', label = 'o(h^2) precision doble')
plt.scatter(np.log(h), np.log(error(o_h2, x0, h, 32)), c = 'b', label = 'o(h^2) precision simple')
plt.plot(np.log(h), np.log(error(o_h4, x0, h, 64)), c = 'g', label = 'o(h^4) precision doble')
plt.scatter(np.log(h), np.log(error(o_h4, x0, h, 32)), c = 'g', label = 'o(h^4) precision simple')


plt.legend(loc='upper left')
plt.show()

'''
Obtengo el mismo resultado que en la teórica

Hay algo más para decir aparte de lo que dice la teórica?
'''