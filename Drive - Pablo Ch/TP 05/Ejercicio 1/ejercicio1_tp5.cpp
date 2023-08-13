//paso todo a arrays nativos

#include <iostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

#include <time.h>

using namespace std;



const int pot = 3;
const int N = pow(2,pot);


int rand2(int rmin, int rmax){
	//retorna un número random entero entre rmin y rmax
	return rand() % (rmax-rmin) + rmin;
}






void RandomWalk(int &x, int &y, int N){
	//toma la posición {x,y} dada por las direcciones de memoria de x e y y modifica la posición a la de los vecinos
	//aleatoriamente
}


void matriz_nula(int matriz[N][N]){
	//recibe la dirección de una matriz cuadrada creada como un array nativo de tamaño N*N
	for(int i = 0; i <= N-1; i++ ){
		for(int j = 0; j <= N-1; j++){
			matriz[i][j] = 0;
		}
	}
}

void imprimir_matriz(int matriz[N][N]){
	//recibe la dirección de una matriz nativa de tamaño N*N y la imprime
	for(int i = 0; i <= N-1; i++ ){
		for(int j = 0; j <= N-1; j++){
			cout << matriz[i][j] << "\t";
		}
		cout << endl;
	}
	return;
}

void imprimir_posiciones(int posiciones[][2], int Nlista){
	for(int i = 0; i <= Nlista-1; i++ ){
		for(int j = 0; j <= 1; j++){
			cout << posiciones[i][j] << "\t";
		}
		cout << endl;
	}
	return;
}

void ListaRandom(int posiciones[][2],int Nlista){
	//dada la dirección de un array nativo {x_vec, y_vec} ambas columnas, aleatoriza los N_rand elementos entre 0 y N-1 para x e y
	cout << "x \t y" << endl;
	for(int i = 0; i <= Nlista-1; i++){
		posiciones[i][0] = rand2(0,N-1);
		posiciones[i][1] = rand2(0,N-1);
	}
}

int pos(int k, int J){
	//Esta función se encarga de hacer el arreglo periódico. J es el tamaño del array.
	//If((k % (J-1)) != 0)
	//	return k % (J-1);
	//else
		return J-1;
}

int main()
{
	srand(time(NULL));
	int x = rand2(1,10);
	cout << x << endl << endl;

	int Ntest = 4;
	int posiciones[Ntest][2];
	ListaRandom(posiciones, Ntest);

	imprimir_posiciones(posiciones, Ntest);

	//El índice del primer elemento de un array es 0. El índice del último elemento de un array de N elementos es N-1.

//	for(int i = N-1; i >= 0; --i ){
//		for(int j = N-1; j >= 0; --j){
//			matriz_nula.at(i).at(j) = i + j;
//		}
//	}
//
//	imprimir_matriz(matriz_nula);



	//Creo semilla y particles como listas aleatorias. La primera con 1 elemento y la segunda con Nparticles elementos.
	//Los elementos que contienen son las posiciones (x,y) en una malla de tamaño size_malla*size_malla
	N


	/*
	Idea de programa:

	Creo semilla y particles como listas aleatorias. La primera con 1 elemento y la segunda con Nparticles elementos.
	Los elementos que contienen son las posiciones (x,y) en una malla de tamaño size_malla*size_malla

	Creo tiempos lista de enteros que contiene en qué paso de tiempo la partícula [i] de particles fue atrapada.
	Debe contener también a los elementos de semilla con tiempo t = 0, por lo que el tamaño de la lista tiempos debe
	ser el tamaño de semilla + particles

	Creo una malla nula size_malla*size_malla. En ella guardaré la información de qué es lo que va ocurriendo paso a paso.
	La malla tiene que ser periódica, así que accederé a sus índices a través de una función pos[] que lo garantiza

	Pinto las semillas sobre la malla con el número 2
	Pinto las partículas sobre la malla con el número 1
	Paso de tiempo:
		Itero sobre las posiciones de semillas y pregunto si alguno de sus primeros vecinos es 1.
			Si hay alguno, lo cambio de color a 2 y lo appendeo en semilla. También agrego el tiempo en el que fue atrapado en
			la lista tiempos
			Si no hay ninguno, no hago nada
		Random Walk
			Recorro los elementos de particles.
				Si no están en semilla, aplico random walk:
					Pinto la casilla en la que están de 0
					Los muevo aleatoriamente a alguna casilla vecina
					Pinto la nueva casilla de 1
				Si están en semilla, no hago nada.

	*/
    return 0;
}
