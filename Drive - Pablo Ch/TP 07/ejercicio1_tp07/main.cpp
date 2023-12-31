/*
Idea de c�digo:
N �tomos en una caja bidimensional LxL
Potencial de Lennard-Jones
U = 4(1/r^12 - 1/r^6)











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

const int Nc = 30;
const int N = Nc*Nc;
const float rho = 0.3;
const float L = sqrt(N/rho);
const float h = 0.0005; //paso
const int Npasos = 20000;
float v0 = 1.1;

//Creo las marices posici�n, velocidad y aceleraci�n. Cada una contiene en cada fila la informaci�n a tiempo t* de todo el sistema.
//Consecuentemente, las matrices tienen tama�o tmax/h x N. Las matrices se crean fuera del main para que no est�n alojadas en el Stack
float mesh_position[Npasos][N][2]; //[tiempo][n�mero de part�cula][x o y]
float mesh_velocity[Npasos][N][2];
float mesh_aceleration[Npasos][N][2];

void matriz_nula(float mesh[Npasos][N][2]){
	//recibe la direcci�n de una matriz creada como un array nativo de tama�o Npasos*N*2 y anula todas sus componentes.
	for(int i = 0; i < Npasos; i++ ){
		for(int j = 0; j < N; j++){
			mesh[i][j][0] = 0; mesh[i][j][1] = 0;
		}
	}
	return;
}

void Ini_position(float mesh_position[Npasos][N][2]){
	//Inicializa las posiciones de todas las part�culas
	//Posiciones iniciales: ri(0) en una red cuadrada: (xi, yi) = (na, nb).
	float a = 1.0*L/(Nc+1);
	int indice = -1;
	for(int i = 0; i < N; i++){
		mesh_position[0][i][0] = a*(i % Nc) + a;
		if(i % Nc == 0){++indice;}
		mesh_position[0][i][1] = a*indice + a;

	}

	return;
}
void Ini_velocity(float mesh_velocity[Npasos][N][2] ){
	//Inicializa las velocidades de todas las part�culas
	//Velocidades iniciales vi(0) aleatorias considerando dos valores elegidos al azar v = (+/-v0, 0) con v0 = 1.1.
	for(int i = 0; i < N; i++){
		mesh_velocity[0][i][0] = v0*((rand() % 2)*2 - 1);
	}

	return;
}


float distancia_ij(float mesh_position[Npasos][N][2], int i,int j, int t){
	//Calcula la distancia entre la part�cula j y la i
	float a1 = mesh_position[t][i][0] - mesh_position[t][j][0];
	float a2 = mesh_position[t][i][1] - mesh_position[t][j][1];
	return sqrt(a1*a1 + a2*a2);
}

float cond_bord(float mesh_position[Npasos][N][2], int t, int i, int j){
	/*Calcula la condici�n de borde para la part�cula i, componente j (x = 0, y = 1).
	En el c�lculo de la aceleraci�n es donde se consideran las condiciones de borde: a la ec. usual se le suman 4 t�rminos que corresponden a las
	interacciones con las paredes simuladas como un potencial de Lennar-Jones s�lo repulsivo, es decir, U_pared = 4/r^12.
	A la aceleraci�n en x le afectan 2 t�rminos correspondientes al potencial repulsivo de las paredes de la izquierda y derecha, mientras que a
	la aceleraci�n en y le afectan los 2 t�rminos restantes.
	*/
	if(j == 1){
		float distancia_UP = L-mesh_position[t][i][j], distancia_DOWN = mesh_position[t][i][j];
		return 4*12*( -1/pow(distancia_UP,13) + 1/pow(distancia_DOWN,13));

	}
	else{
		float distancia_RIGHT = L-mesh_position[t][i][j], distancia_LEFT = mesh_position[t][i][j];
		return 4*12*( 1/pow(distancia_LEFT,13) -  1/pow(distancia_RIGHT,13) );
	}
}

void Aceleration(float mesh_position[Npasos][N][2], float mesh_aceleration[Npasos][N][2], int t){
	/*
	Para dado t (n�mero de fila de mesh_aceleration), calcula las aceleraciones de todas las part�culas usando la expresi�n escrita en mis apuntes.
	En el c�lculo de la aceleraci�n es donde se consideran las condiciones de borde: a la ec. usual se le suman 4 t�rminos que corresponden a las
	interacciones con las paredes simuladas como un potencial de Lennar-Jones s�lo repulsivo, es decir, U_pared = 4/r^12.
	*/
	double rij;
	for(int i = 0; i < N; ++i){
		//Aceleraci�n debido a los bordes:
		mesh_aceleration[t][i][0] += cond_bord(mesh_position, t, i, 0);
		mesh_aceleration[t][i][1] += cond_bord(mesh_position, t, i, 1);
		//Aceleraci�n debido a otras part�culas:
		for(int j = 0; j < N; ++j){
			if(i == j){
				continue; //sumo sobre todos los i!=j
			}
			else{
				rij = distancia_ij(mesh_position, i, j, t);
				mesh_aceleration[t][i][0] += -(mesh_position[t][i][0] - mesh_position[t][j][0])*24*(1/pow(rij,8) - 2/pow(rij,14));
				mesh_aceleration[t][i][1] += -(mesh_position[t][i][1] - mesh_position[t][j][1])*24*(1/pow(rij,8) - 2/pow(rij,14));
			}
		}
	}
	return;
}


void Verlet(float mesh_position[Npasos][N][2], float mesh_velocity[Npasos][N][2], float mesh_aceleration[Npasos][N][2], int t){
	//Calcula las nuevas posiciones, aceleraciones y velocidades para t;
	for(int i = 0; i < N; ++i){
		mesh_position[t][i][0] = mesh_position[t-1][i][0] + h*mesh_velocity[t-1][i][0] + (h*h/2)*mesh_aceleration[t-1][i][0];
		mesh_position[t][i][1] = mesh_position[t-1][i][1] + h*mesh_velocity[t-1][i][1] + (h*h/2)*mesh_aceleration[t-1][i][1];
	}

	Aceleration(mesh_position, mesh_aceleration, t);

	for(int i = 0; i < N; ++i){
		mesh_velocity[t][i][0] = mesh_velocity[t-1][i][0] + (h/2)*(mesh_aceleration[t-1][i][0] + mesh_aceleration[t][i][0]);
		mesh_velocity[t][i][1] = mesh_velocity[t-1][i][1] + (h/2)*(mesh_aceleration[t-1][i][1] + mesh_aceleration[t][i][1]);
	}
	return;
}
float Energia_cinetica(float mesh_velocity[Npasos][N][2], int t){
	//Devuelve la energ�a cin�tica de todo el sistema a tiempo t
	float ener = 0;
	for(int i = 0; i < N; ++i){
		ener += (mesh_velocity[t][i][0]*mesh_velocity[t][i][0] + mesh_velocity[t][i][1]*mesh_velocity[t][i][1])/2; //1/2 v^2
	}
	return ener;
};

float U(float r){
	return 4*(1/pow(r,12) - 1/pow(r,6));
};

float U_borde(float r){
	return 4/pow(r,12);
};

float Energia_potencial(float mesh_position[Npasos][N][2], int t){
	//Devuelve la energ�a potencial de todo el sistema a tiempo t
	float ener = 0;
	float rij;
	for(int i = 0; i < N; ++i){
	//suma la energ�a con los bordes
	ener += U_borde(L-mesh_position[t][i][1]) + U_borde(mesh_position[t][i][1]) + U_borde(L-mesh_position[t][i][0]) + U_borde(mesh_position[t][i][0]);
	//sumo las energ�as entre las part�culas
	for(int j = 0; j < N; ++j){
		if(i == j){
			continue; //sumo sobre todos los i!=j
		}
		else{
			rij = distancia_ij(mesh_position, i, j, t);
			ener += U(rij)/2; //La divido por 2 porque en la suma voy a contar 2 veces cada interacci�n
		}
	}
	}


	return ener;
};

int main()
{
	//Inicializo el generador de n�meros random
	srand(time(NULL));

	//Anulo todas las matrices para no tener problema con componentes mal inicializadas:
	matriz_nula(mesh_position); matriz_nula(mesh_velocity); matriz_nula(mesh_aceleration);


	//Creo arrays nativos E_kin y E_pot que contienen la energ�a cin�tica y potencial del sistema a cada tiempo. Los inicializo en cero.
	float E_kin[Npasos];
	float E_pot[Npasos];
	for(int i = 0; i < Npasos; ++i){
		E_kin[i] = 0; E_pot[i] = 0;
	}



	//Inicializo posici�n y velocidad
	Ini_position(mesh_position);
	Ini_velocity(mesh_velocity);

	//Calculo la energ�a cin�tica y potencial a t = 0
	E_kin[0] = Energia_cinetica(mesh_velocity, 0);
	E_pot[0] = Energia_potencial(mesh_position, 0);

	cout << "Energia a t = 0: " << "\nCalculo: " << E_kin[0] << "\nAnaliticamente: " << N*(v0*v0)/2 << endl;

	//Inicializo aceleraci�n usando los valores de position y velocity
	Aceleration(mesh_position, mesh_aceleration, 0);

	//Usando Verlet calculo las nuevas posiciones hasta t = tmax
	for(int t = 1; t < Npasos; ++t){
		Verlet( mesh_position, mesh_velocity, mesh_aceleration, t);
		//Calculo las energ�as cin�tica y potencial:
		E_kin[t] = Energia_cinetica(mesh_velocity, t);
		E_pot[t] = Energia_potencial(mesh_position, t);
		if(t%100 == 0){
			cout << "Paso: " << t << endl;
		}
	}

	cout << "Par�metros: \nN = " << N << "\nL = " << L << "\nh = " << h << "\nPasos = " << Npasos;

	//Guardo los datos de las posiciones para graficar con Mathematica

	ofstream oFilex, oFiley;
	oFilex.open("mesh_position_x.dat"); oFiley.open("mesh_position_y.dat");
	if(oFilex.is_open() && oFiley.is_open()){
		for(int i = 0; i < Npasos; ++i){
			for(int j = 0; j < N; ++j){
				oFilex << mesh_position[i][j][0] << "\t";
				oFiley << mesh_position[i][j][1] << "\t";
			}
			oFilex << endl; oFiley << endl;
		}
		oFilex.close(); oFiley.close();
	}

	cout << endl << "Posiciones guardadas.";

	//Guardo las energ�as cin�tica y potencial para graficar con Mathematica:
	ofstream oFile_Ener;
	oFile_Ener.open("Energias.dat");
	if(oFile_Ener.is_open()){
		for(int i = 0; i < Npasos; ++i){
			oFile_Ener << E_kin[i] << "\t" << E_pot[i] << endl;
		}
		oFile_Ener.close();
	}

	cout << endl << "Energ�as guardadas.";

	//Guardo las velocidades para trabajar luego las distribuciones de velocidades en Mathematica:

	ofstream oFilevx, oFilevy;
	oFilevx.open("mesh_velocity_x.dat"); oFilevy.open("mesh_velocity_y.dat");
	if(oFilevx.is_open() && oFilevy.is_open()){
		for(int i = 0; i < Npasos; ++i){
			for(int j = 0; j < N; ++j){
				oFilevx << mesh_velocity[i][j][0] << "\t";
				oFilevy << mesh_velocity[i][j][1] << "\t";
			}
			oFilevx << endl; oFilevy << endl;
		}
		oFilevx.close(); oFilevy.close();
	}

	cout << endl << "Velocidades guardadas.";

    return 0;
}
