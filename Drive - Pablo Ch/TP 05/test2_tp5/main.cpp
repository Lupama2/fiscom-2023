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


struct Ranq1 { //Recommended generator for everyday use. The period is ~1.8E19, so it should not be used by an application that makes more than 1E12 calls.
	unsigned long long int v;
	Ranq1(unsigned long long int j) : v(4101842887655102017LL) {
		v ^= j;
		v = int64();
	}
	inline unsigned long long int int64() {//Return 64-bit random integer
		v ^= v >> 21; v ^= v << 35; v ^= v >> 4;
		return v * 2685821657736338717LL;
	}
	inline double doub() { return 5.42101086242752217E-20 * int64(); }//Return random double-precision floating value in the range 0. to 1
	inline unsigned int int32() { return (unsigned int)int64(); } //Return 32-bit random integer
};


Ranq1 ran(100);

int rand2(int rmin, int rmax){
	//retorna un n�mero random entero entre rmin y rmax
	unsigned int aleatorio = ran.int32();
	return (aleatorio % (rmax-rmin + 1))+ rmin;
	//return rand() % (rmax-rmin + 1) + rmin;
}
//int rand2(int rmin, int rmax){
//	//retorna un n�mero random entero entre rmin y rmax
//	int x = rand2();
//	return int(100000000*x) % (rmax-rmin) + rmin;
//}


void RandomWalk(int &x, int &y, int N){
	//toma la posici�n {x,y} dada por las direcciones de memoria de x e y y modifica la posici�n a la de los vecinos
	//aleatoriamente
}


void matriz_nula(int matriz[N][N]){
	//recibe la direcci�n de una matriz cuadrada creada como un array nativo de tama�o N*N
	for(int i = 0; i <= N-1; i++ ){
		for(int j = 0; j <= N-1; j++){
			matriz[i][j] = 0;
		}
	}
}

void imprimir_matriz(int matriz[N][N]){
	//recibe la direcci�n de una matriz nativa de tama�o N*N y la imprime
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
	//dada la direcci�n de un array nativo {x_vec, y_vec} ambas columnas, aleatoriza los N_rand elementos entre 0 y N-1 para x e y
	cout << "x \t y" << endl;
	for(int i = 0; i <= Nlista-1; i++){
		posiciones[i][0] = rand2(0,N-1);
		posiciones[i][1] = rand2(0,N-1);
	}
}

int pos(int k, int J){
	//Esta funci�n se encarga de hacer el arreglo peri�dico. J es el tama�o del array.
	//If((k % (J-1)) != 0)
	//	return k % (J-1);
	//else
		return J-1;
}

int main()
{

	//Pruebo rank1 de Numerical Recipies:
	unsigned int r;
	int Nmax = 10;
	for(int i=0; i<=Nmax; i++){
		r = ran.int32();
		cout << r << endl;
	}
//	srand(time(NULL));
//	int x = rand2(1,10);
//	cout << x << endl << endl;
//
//	int Ntest = 4;
//	int posiciones[Ntest][2];
//	ListaRandom(posiciones, Ntest);
//
//	imprimir_posiciones(posiciones, Ntest);

	//El �ndice del primer elemento de un array es 0. El �ndice del �ltimo elemento de un array de N elementos es N-1.

//	for(int i = N-1; i >= 0; --i ){
//		for(int j = N-1; j >= 0; --j){
//			matriz_nula.at(i).at(j) = i + j;
//		}
//	}
//
//	imprimir_matriz(matriz_nula);


	/*
	Idea de programa:

	Creo semilla y particles como listas aleatorias. La primera con 1 elemento y la segunda con Nparticles elementos.
	Los elementos que contienen son las posiciones (x,y) en una malla de tama�o size_malla*size_malla

	Creo tiempos lista de enteros que contiene en qu� paso de tiempo la part�cula [i] de particles fue atrapada.
	Debe contener tambi�n a los elementos de semilla con tiempo t = 0, por lo que el tama�o de la lista tiempos debe
	ser el tama�o de semilla + particles

	Creo una malla nula size_malla*size_malla. En ella guardar� la informaci�n de qu� es lo que va ocurriendo paso a paso.
	La malla tiene que ser peri�dica, as� que acceder� a sus �ndices a trav�s de una funci�n pos[] que lo garantiza

	Pinto las semillas sobre la malla con el n�mero 2
	Pinto las part�culas sobre la malla con el n�mero 1
	Paso de tiempo:
		Itero sobre las posiciones de semillas y pregunto si alguno de sus primeros vecinos es 1.
			Si hay alguno, lo cambio de color a 2 y lo appendeo en semilla. Tambi�n agrego el tiempo en el que fue atrapado en
			la lista tiempos
			Si no hay ninguno, no hago nada
		Random Walk
			Recorro los elementos de particles.
				Si no est�n en semilla, aplico random walk:
					Pinto la casilla en la que est�n de 0
					Los muevo aleatoriamente a alguna casilla vecina
					Pinto la nueva casilla de 1
				Si est�n en semilla, no hago nada.

	*/
    return 0;
}
