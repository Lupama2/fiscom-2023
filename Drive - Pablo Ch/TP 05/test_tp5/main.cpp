#include <iostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

const int N = 10;

void matriz_nula(N){
	//recibe la dirección de una matriz de tamaño N y asigna elementos nulos
}

void imprimir_matriz(array<array<double, N>, N> &matriz){
	// imprime una matriz CUADRADA con N filas y N columnas
	for(int i = N-1; i >= 0; i-- ){
		for(int j = N-1; j >= 0; j--){
			cout << matriz.at(i).at(j) << "\t";
		}
		cout << endl;
	}
	return;
}

void ran2(){
	//retorna un número random
}

array<double, > = ListaRandom



int main()
{
	array<array<double,N>, N> matriz_nula;
	//El índice del primer elemento de un array es 0. El índice del último elemento de un array de N elementos es N-1.

	for(int i = N-1; i >= 0; --i ){
		for(int j = N-1; j >= 0; --j){
			matriz_nula.at(i).at(j) = i + j;
		}
	}

	imprimir_matriz(matriz_nula);

    return 0;
}
