/*
Idea de programa:

Carga los parámetros de un archivo externo. Parámetros:
* Nseed
* Nparticles
* size_malla

Creo semilla y particles como listas aleatorias. La primera con 1 elemento y la segunda con Nparticles elementos.
Los elementos que contienen son las posiciones (x,y) en una malla de tamaño Nmalla*Nmalla

Creo tiempos lista de enteros que contiene en qué paso de tiempo la partícula [i] de particles fue atrapada.
Debe contener también a los elementos de semilla con tiempo t = 0, por lo que el tamaño de la lista tiempos debe
ser el tamaño de semilla + particles

Creo una malla nula Nmalla*Nmalla. En ella guardaré la información de qué es lo que va ocurriendo paso a paso.
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

Exporta las listas seed y tiempos a un .txt


FALTA:
Considere que los caminantes quedan pegados a la estructura por
contactos a lo largo de los enlaces de la red y a 45◦
.

*/

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
#include <typeinfo>


using namespace std;


const int potencia = 10;//10;
const int Nmalla = pow(2,potencia);
const int Nseed = 1, Npart = 20000;
const int tmax = 8*pow(10,5);//pow(10,4);//3*pow(10,5);//pow(10,4); //en teoría, según Chule, necesitaría en torno a 500.000 pasos
const bool imprimir = false; //si quiero imprimir la malla en cmd


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
	//retorna un número random entero entre rmin y rmax
	unsigned int aleatorio = ran.int32();
	return (aleatorio % (rmax-rmin + 1))+ rmin;
	//return rand() % (rmax-rmin + 1) + rmin;
}

void matriz_nula(int matriz[Nmalla][Nmalla]){
	//recibe la dirección de una matriz cuadrada creada como un array nativo de tamaño Nmalla*Nmalla
	for(int i = 0; i < Nmalla; i++ ){
		for(int j = 0; j < Nmalla; j++){
			matriz[i][j] = 0;
		}
	}
}

void imprimir_matriz(int matriz[Nmalla][Nmalla]){
	//recibe la dirección de una matriz nativa de tamaño N*N y la imprime
	for(int i = 0; i < Nmalla; i++ ){
		for(int j = 0; j < Nmalla; j++){
			cout << matriz[i][j] << "\t";
		}
		cout << endl;
	}
	return;
}

void imprimir_posiciones(int posiciones[][2], int Nlista){
	cout << "x \t y" << endl;
	for(int i = 0; i < Nlista; i++ ){
		for(int j = 0; j < 2; j++){
			cout << posiciones[i][j] << "\t";
		}
		cout << endl;
	}
	return;
}

void ListaRandom(int posiciones[][2],int Nlista){
	//dada la dirección de un array nativo de tamaño Nlista * 2: {x_vec, y_vec} ambas columnas,
	//aleatoriza los N_rand elementos entre 0 y Nmalla-1 para x e y
	for(int i = 0; i < Nlista; i++){
		posiciones[i][0] = rand2(0,Nmalla-1);
		posiciones[i][1] = rand2(0,Nmalla-1);
	}
}

int pos(int k){
	//Esta función se encarga de hacer el arreglo periódico.
	return (k+Nmalla) % Nmalla;
}

void primeros_vecinos(int vecinos[4][2], int x, int y){
	//Toma la lista vecinos y cambia los elementos por las posiciones de los vecinos de (xseed, yseed)
	vecinos[0][0] = pos(x-1); vecinos[0][1] = y; //arriba
	vecinos[1][0] = pos(x+1); vecinos[1][1] = y; //abajo
	vecinos[2][0] = x; vecinos[2][1] = pos(y-1); //izquierda
	vecinos[3][0] = x; vecinos[3][1] = pos(y+1); //derecha
	return;
}

void segundos_vecinos(int vecinos[8][2], int x, int y){
	//Toma la lista vecinos y cambia los elementos por las posiciones de los vecinos de (xseed, yseed)
	vecinos[0][0] = pos(x-1); vecinos[0][1] = y; //arriba
	vecinos[1][0] = pos(x+1); vecinos[1][1] = y; //abajo
	vecinos[2][0] = x; vecinos[2][1] = pos(y-1); //izquierda
	vecinos[3][0] = x; vecinos[3][1] = pos(y+1); //derecha
	vecinos[4][0] = pos(x-1); vecinos[4][1] = pos(y+1); //arriba + derecha
	vecinos[5][0] = pos(x-1); vecinos[5][1] = pos(y-1); //arriba + izquierda
	vecinos[6][0] = pos(x+1); vecinos[6][1] = pos(y+1); //abajo + derecha
	vecinos[7][0] = pos(x+1); vecinos[7][1] = pos(y-1); //abajo + izquierda
	return;
}

void pintar(int dt, int &Natrapadas, int seed[][2], int tiempos[], int malla[Nmalla][Nmalla]){
	//Itero sobre las posiciones de semillas y pregunto si alguno de sus primeros vecinos es 10.
	//	Si hay alguno, lo cambio de color a 20 y lo appendeo en semilla. También agrego el tiempo en el que fue atrapado en
	//	la lista tiempos
	//	Si no hay ninguno, no hago nada
	int xseed, yseed, xvecino, yvecino;
	int vecinos[8][2];
	for(int i = 0; i < Natrapadas;i++){
		xseed = pos(seed[i][0]); yseed = pos(seed[i][1]);
		segundos_vecinos(vecinos, xseed, yseed);
		for(int j = 0; j < 8; ++j){
			xvecino = pos(vecinos[j][0]); yvecino = pos(vecinos[j][1]);
			if(malla[xvecino][yvecino] == 10){
				malla[xvecino][yvecino] = 20;
				seed[Natrapadas][0] = xvecino;
				seed[Natrapadas][1] = yvecino;
				tiempos[Natrapadas] = dt;
				++Natrapadas;
			}
		}
	}
}

void RandomWalk(int i,int particles[][2], int malla[Nmalla][Nmalla]){
	int xpart = particles[i][0], ypart = particles[i][1];
	//Pinto la casilla en la que están de 0
	malla[xpart][ypart] = 0;
	//Los muevo aleatoriamente a alguna casilla vecina
	int vecinos[4][2]; primeros_vecinos(vecinos, xpart, ypart);
	int aleatorio = ran.int32() % 4;
	int xnew = vecinos[aleatorio][0], ynew = vecinos[aleatorio][1];
	particles[i][0] = xnew; particles[i][1] = ynew;
	//Pinto la nueva casilla de 10
	malla[xnew][ynew] = 10;
}

void Randomwalk_gral(int Npart, int particles[][2], int malla[Nmalla][Nmalla]){
	//Recorro los elementos de particles.
	//	Si no están en semilla, aplico random walk:
	//	Si están en semilla, no hago nada.
	int xpart, ypart;
	for(int i = 0; i < Npart; i++){
		xpart = particles[i][0], ypart = particles[i][1];
		if(malla[xpart][ypart] != 20){
			RandomWalk(i, particles, malla);
		}
	}
	return;
}

//Creo la matriz afuera del main para que no esté alojada en el Stack.
int malla[Nmalla][Nmalla];
int Natrapadas = Nseed;
int seed[Nseed + Npart][2], particles[Npart][2];

int main()
{
	//srand(time(NULL));
	cout << "Nro de semillas: " << Nseed << "\nNro de particulas: " << Npart << "\nCantidad de pasos(tmax): " << tmax
	<< "\nSize de malla: " << Nmalla << endl << endl;

	//Creo semilla y particles como listas aleatorias. La primera con Nseed+Npart elemento y la segunda con Npart elementos.
	//Los elementos que contienen son las posiciones (x,y) en una malla de tamaño Nmalla*Nmalla

	//ListaRandom(seed,Nseed);
	ListaRandom(particles,Npart);

	//Posiciono la semilla en el medio:
	seed[0][0] = Nmalla/2; seed[0][1] = Nmalla/2;

	//Creo tiempos lista de enteros inicializada en cero que contiene en qué paso de tiempo la partícula [i] de particles
	//fue atrapada. Debe contener también a los elementos de semilla con tiempo t = 0, por lo que el tamaño de la
	//lista tiempos debe ser el tamaño de semilla + particles
	int Ntiempos = Nseed + Npart;
	int tiempos[Ntiempos];
	for(int i = 0; i < Nseed;i++){
		tiempos[i] = 1;
	}

	//Creo una malla nula Nmalla*Nmalla. En ella guardaré la información de qué es lo que va ocurriendo paso a paso.
	//La malla tiene que ser periódica, así que accederé a sus índices a través de una función pos[] que lo garantiza
	matriz_nula(malla);

	//Pinto las partículas sobre la malla con el número 10
	for(int i = 0; i <Npart;i++){
		malla[pos(particles[i][0])][pos(particles[i][1])] = 10;
	}
	//Pinto las semillas sobre la malla con el número 20
	for(int i = 0; i <Nseed;i++){
		malla[pos(seed[i][0])][pos(seed[i][1])] = 20;
	}
	/*Por cómo está hecho el código, puede ser que
	(1) 2 posiciones de partículas son las mismas. En tal caso, se pisan
	(2) alguna partícula coincida con seed. En tal caso, seed la pisa
	*/
	if(imprimir == true){
		imprimir_matriz(malla);
	}

	for(int dt = 1; dt <= tmax; dt++){
		//Paso de tiempo:
		pintar(dt, Natrapadas, seed, tiempos, malla);
		Randomwalk_gral(Npart, particles, malla);
		//cout << "\nCambio: " << dt << endl;
		//imprimir_matriz(malla);
		if(Natrapadas % 500 == 0){
				cout << endl << "Atrapadas: " << Natrapadas;
		}
	}

	cout << "\nAtrapadas: " << Natrapadas << endl;
	if(imprimir == true){
		cout << "\nFinal: " << endl;
		imprimir_matriz(malla);
		cout << "x \t y \t t" << endl;
		for(int i = 0; i <= Natrapadas - 1; ++i){
			cout << seed[i][0] << "\t" << seed[i][1] << "\t" << tiempos[i] << endl;
		}
	}
	ofstream oFile;
	oFile.open("kk.dat");
	if(oFile.is_open()){
		//oFile << "x \t y \t t \n";
		for(int i = 0; i <= Natrapadas - 1; ++i){
		oFile << seed[i][0] << "\t" << seed[i][1] << "\t" << tiempos[i] << endl;
		}
		oFile.close();
	}

	ofstream oFile2;
	oFile2.open("kk_particles.dat");
	if(oFile2.is_open()){
		//oFile << "x \t y";
		for(int i = 0; i < Npart; ++i){
		oFile2 << particles[i][0] << "\t" << particles[i][1] << endl;
		}
		oFile2.close();
	}



    return 0;
}
