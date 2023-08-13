/*
Se resolvi� num�ricamente la ecuaci�n de Schr�dinger unidimensional empleando el m�todo de Crank-Nicholson.
Se emple� un paquete gaussiano con distintas energ�as como condici�n inicial y condiciones de borde nulas.
Se resolvi� el problema para potencial nulo (part�cula libre) y para una barrera de potencial finito.
Se emple� la  respresentaci�n de Cayley del operador unitario de evoluci�n temporal.


Para usar n�meros complejos:
https://en.cppreference.com/w/cpp/numeric/complex

Energ�as: considero E = nu^2 k0^2
1) U = 0, k0 = 25 PI
2)
i) E < U0, k0 = 25 PI
ii) E <= U0, k0 = 35 sqrt(2) PI
iii) E > U0, k0 = 50 sqrt(2) PI

*/
#include <iostream>
//Paquetes para usar n�meros complejos: recordar que la "i" sola no funciona, siempre hay que acompa�arla de un nro como "1i".
#include <iomanip>
#include <complex>
#include <cmath>
//Para manejar archivos:
#include <iostream>
#include <iostream>
#include <fstream>


using namespace std;
//Par�metros:
const double L = 1;


const complex <double> k0 = 50*sqrt(2)*M_PI;
const double k0_real = 50*sqrt(2)*M_PI; //id�ntico al anterior pero como variable real
const double tmax = 1.9*(L/2)/(k0_real); //hasta epsilon*epsilon/delta funciona bien.

const double epsilon = 0.001; //discretizaci�n espacial
const double delta = 2*epsilon*epsilon; //discretizaci�n temporal
const complex <double> x0 = 0.25;
const complex <double> sigma0 = 0.05;


const double lambda = 2*epsilon*epsilon/delta;
const int J = L/epsilon;
const int N = tmax/delta;

//Creo una malla de n�meros complejos donde guardar� los datos de Phi para todo x y para todo t.
complex<double> malla[J][N];
complex <double> initial_list[J];


double integral_trapezoid(double a, double b, double f_a, double f_b){
	//Calcula la integral de f(x) entre a y b usando la regla del trapezoide
	return (b-a)*(f_a + f_b)/2;
}

double integral(double lista[], int n_lista){
	//Calcula la integral discreta de una lista de tama�o n-lista
	double suma = 0;
	for(int i = 0; i < n_lista; ++i){
		suma += integral_trapezoid(i*epsilon, (i+1)*epsilon, lista[i], lista[i+1]);
	}
	return suma;
}

double norma(complex<double> malla[J][N], int n){
	//Calcula la norma de la funci�n a tiempo n
	complex <double> integrando;
	double norma_list[J];
	for(int j = 0; j < J; ++j){
		norma_list[j] = real(conj(malla[j][n])*malla[j][n]); //Ya s� que es real el resultado. Le agrego real() para que el
		//resultado me lo tire como double y no como complex double
	}
	return integral(norma_list, J);
}

void matriz_nula(complex<double> matriz[J][N]){
	//recibe la direcci�n de una matriz cuadrada creada como un array nativo de tama�o Nmalla*Nmalla
	for(int i = 0; i < J; i++ ){
		for(int j = 0; j < N; j++){
			matriz[i][j] = 0;
		}
	}
}

void Phi_inicializer(complex<double> malla[J][N]){
	//Calcula la condici�n inicial y la guarda en el array nativo initial_list.
	complex <double> C0 = 1;
	double x;
	for(int i = 0; i < J; ++i){
		x = i*epsilon;
		initial_list[i] = C0*exp(1i*k0*x)*exp(-(x-x0)*(x-x0)/(4.*sigma0*sigma0));
	}
	//Calculo la norma:
	complex <double> integrando;
	double norma_list[J];
	for(int j = 0; j < J; ++j){
		norma_list[j] = abs(conj(initial_list[j])*initial_list[j]);
	}
	C0 = integral(norma_list, J);

	//Renormalizo:
	for(int j = 0; j < J; ++j){
		initial_list[j] = initial_list[j]/(sqrt(C0));
	}

	//Carga los datos de Phi_ini
	for(int i = 0; i < J; ++i){
		malla[i][0] =  initial_list[i];
	}
	return;
}

double V(int j){
	//calcula el valor del potencial en x = j*epsilon

	//Potencial nulo:
	//return 0;

	//Barrera de potencial:
	double a = 0.032;
	double x = j*epsilon;
	if(abs(x - L/2)<a){
		return 50*M_PI*50*M_PI;
	}
	return 0;
}

complex <double> Omega(complex<double> malla[J][N], int j, int n){
	return - malla[j+1][n] + (1i*lambda + epsilon*epsilon*V(j) + 2.)*malla[j][n] - malla[j-1][n];
}

complex <double> f(complex<double> malla[J][N],complex<double> f_vec[J],complex<double> e_vec[J],complex<double> omega_j, int j){
	return omega_j + f_vec[j-1]/e_vec[j-1];

}

complex <double> e(complex <double> malla[J][N], complex<double> e_vec[J], int j){
	return 2. + epsilon*epsilon*V(j) - 1i*lambda - 1./e_vec[j-1];
}

void paso_n(complex<double> malla[J][N], int nsig){
	//Creo los vectores e_vec, f_vec y Omega_vec donde guardar� los datos de e_j, f_j^n
	complex<double> e_vec[J], f_vec[J];
	//Calculo e_vec y f_vec:
	//N�tese que no nos interesa el primer valor, j = 0 ni el �ltimo valor, J. Ya conocemos los valores de Phi all� por las condiciones de borde.
	//Primero calculo e_vec[1] y f_vec[1]:
	e_vec[1] = 2. + epsilon*epsilon*V(1) - 1i*lambda;
	f_vec[1] = Omega(malla, 1, nsig-1);
	for(int j = 2; j < J; ++j){
		complex<double> omega_j = Omega(malla, j, nsig-1);
		f_vec[j] = f(malla, f_vec, e_vec, omega_j, j);
		e_vec[j] = e(malla, e_vec, j);
	}

	//Calculo Phi_j^{nsig} para todo j comenzando desde j = J-1 y terminando en j = 1.
	for(int j = J-2; j > 0; --j ){
		malla[j][nsig] = (malla[j+1][nsig] - f_vec[j])/e_vec[j];
	}
	return;
}

complex <double> x_mean(complex<double> malla[J][N], int n){
	//Calcula el valor medio de x a tiempo n
	complex <double> integrando;
	double x_mean_list_real[J], x_mean_list_imag[J];
	for(int j = 0; j < J; ++j){
		integrando = conj(malla[j][n])*(j*epsilon)*malla[j][n];
		x_mean_list_real[j] = real(integrando);
		x_mean_list_imag[j] = imag(integrando);
	}
	return integral(x_mean_list_real, J) + 1i*integral(x_mean_list_imag,J);
}

complex <double> x2_mean(complex<double> malla[J][N], int n){
	//Calcula el valor medio de x^2 a tiempo n
	complex <double> integrando;
	double x2_mean_list_real[J], x2_mean_list_imag[J];
	for(int j = 0; j < J; ++j){
		integrando = conj(malla[j][n])*(j*epsilon)*(j*epsilon)*malla[j][n];
		x2_mean_list_real[j] = real(integrando);
		x2_mean_list_imag[j] = imag(integrando);
	}
	return integral(x2_mean_list_real, J) + 1i*integral(x2_mean_list_imag,J);
}

double x_var(complex<double> malla[J][N], int n){
	//Calcula la dispersi�n de x (varianza) a tiempo n.
	return abs(x2_mean(malla, n)) - abs(x_mean(malla, n))*abs(x_mean(malla, n)); //desprecio la parte imaginaria asumiendo que se debe a errores de redondeo
}

complex <double> p_mean(complex<double> malla[J][N], int n){
	/*Calcula el valor medio de p a tiempo n usando la expresi�n 1.7 para estimar la derivada primera.
	df/dx = (f_{j+1}-f_{j-1})/2epsilon
	Los puntos de los extremos no s econsideran. Esto se justifica en que seg�n Goldberg, el programa est� hecho para que la funci�n de onda
	nunca llegue a los extremos, dado que pueden surgir problemas (con las condiciones de contorno quiz�s).
	p = -i nu d/dx, nu = h_bar = 1 en nuestro c�lculo
	*/
	complex <double> integrando;
	double p_mean_list_real[J], p_mean_list_imag[J];
	//Fijo los extremos en cero para que no haya problemas de inicializaci�n:
	p_mean_list_real[0] = 0;
	p_mean_list_real[J] = 0;
	p_mean_list_imag[0] = 0;
	p_mean_list_imag[J] = 0;

	for(int j = 1; j < J-1; ++j){
		integrando = conj(malla[j][n])*(-1i)*(malla[j+1][n] - malla[j-1][n])/(2*epsilon);
		p_mean_list_real[j] = real(integrando);
		p_mean_list_imag[j] = imag(integrando);
	}
	return integral(p_mean_list_real, J) + 1i*integral(p_mean_list_imag, J);
}

complex <double> p2_mean(complex<double> malla[J][N], int n){
	/*Calcula el valor medio de p2 a tiempo n usando la expresi�n 1.8 para estimar la derivada segunda.
	d2f/dx2 = (f_{j+1} - 2f_j + f_{j-1})/epsilon^2
	p = -i nu d/dx, nu = h_bar = 1 en nuestro c�lculo
	*/

	//Calcula el valor medio de p^2 a tiempo n.

	complex <double> integrando;
	double p2_mean_list_real[J], p2_mean_list_imag[J];
	//Fijo los extremos en cero para que no haya problemas de inicializaci�n:
	p2_mean_list_real[0] = 0;
	p2_mean_list_real[J] = 0;
	p2_mean_list_imag[0] = 0;
	p2_mean_list_imag[J] = 0;

	for(int j = 1; j < J-1; ++j){
		integrando = conj(malla[j][n])*(-1i)*(-1i)*(malla[j+1][n] - 2.*malla[j][n] + malla[j-1][n])/(epsilon*epsilon);
		p2_mean_list_real[j] = real(integrando);
		p2_mean_list_imag[j] = imag(integrando);
	}
	return integral(p2_mean_list_real, J) + 1i*integral(p2_mean_list_imag, J);
}

double p_var(complex<double> malla[J][N], int n){
	//Calcula la dispersi�n de x (varianza) a tiempo n.
	return abs(p2_mean(malla, n)) - abs(p_mean(malla, n))*abs(p_mean(malla, n)); //Calculo usando valor absoluto!
}

complex <double> Energy(complex<double> malla[J][N], int n){
	complex <double> integrando;
	double potential_list_real[J], potential_list_imag[J];
	for(int j = 0; j < J; ++j){
		integrando = conj(malla[j][n])*V(j)*malla[j][n];
		potential_list_real[j] = real(integrando);
		potential_list_imag[j] = imag(integrando);
	}
	complex <double> potential = integral(potential_list_real, J) + 1i*integral(potential_list_imag, J);
	return p2_mean(malla, n) + potential;
}



int main()
{
	//Imprimo par�metros utilizados:
	cout << "J = " << J << "\nN = " << N << endl;

	//Inicializo la malla en cero
	matriz_nula(malla);

	//Inicializo la malla a t = 0
    Phi_inicializer(malla);


    //Avanzo en el tiempo:
    //cout << "\nPhi(1) = ";
    for(int n = 1; n < N; ++n){
		paso_n(malla, n);
		cout << 100*(n+1)/N << "%\n";
    }
    //imprimir_matriz(malla);

	//Guardo la funci�n Phi para todo j para todo n:
	ofstream oFileReal, oFileImag;
	oFileReal.open("malla_real.dat", std::ofstream::app);
	oFileImag.open("malla_imag.dat", std::ofstream::app);
	if(oFileReal.is_open() && oFileImag.is_open()){
		for(int j = 0; j < J; ++j){
			for(int n = 0; n < N; ++n){
				oFileReal << real(malla[j][n]) << "\t"; oFileImag << imag(malla[j][n]) << "\t";
				//oFile << real(malla[j][n]) << " + (" << imag(malla[j][n]) << ") I" << "\t";
			}
			oFileReal << endl; oFileImag << endl;
		}
		oFileReal.close(); oFileImag.close();
	}

	ofstream OFile_Parameters;
	OFile_Parameters.open("parametros.dat");
	if(OFile_Parameters.is_open()){
		OFile_Parameters << "k0 = " << k0_real << "\nN = " << N << "\nEnergia a tiempo inicial: " << real(Energy(malla, 0));
	}

	ofstream OFile_MeanValues_real, OFile_MeanValues_imag;
	OFile_MeanValues_real.open("valores_medios_real.dat", std::ofstream::app); OFile_MeanValues_imag.open("valores_medios_imag.dat", std::ofstream::app);
	if(OFile_MeanValues_real.is_open() && OFile_MeanValues_imag.is_open()){
		//Energ�a:
		for(int n = 0; n < N; ++n){
			OFile_MeanValues_real << real(Energy(malla, n)) << "\t"; OFile_MeanValues_imag << imag(Energy(malla, n)) << "\t";
		}
		OFile_MeanValues_real << endl; OFile_MeanValues_imag << endl;
		cout << "\nEnergia guardada";
		//Norma:
		for(int n = 0; n < N; ++n){
			OFile_MeanValues_real << norma(malla, n) << "\t"; OFile_MeanValues_imag << 0 << "\t";
		}
		OFile_MeanValues_real << endl; OFile_MeanValues_imag << endl;
		cout << "\nNorma guardada";
		//Valor medio de x:
		for(int n = 0; n < N; ++n){
			OFile_MeanValues_real << real(x_mean(malla, n)) << "\t"; OFile_MeanValues_imag << imag(x_mean(malla, n)) << "\t";
		}
		OFile_MeanValues_real << endl; OFile_MeanValues_imag << endl;

		//Varianza de x:
		for(int n = 0; n < N; ++n){
			OFile_MeanValues_real << x_var(malla,n) << "\t"; OFile_MeanValues_imag << 0 << "\t";
		}
		OFile_MeanValues_real << endl; OFile_MeanValues_imag << endl;
		cout << "\nPosiciones guardadas";
		//Valor medio de p:
		for(int n = 0; n < N; ++n){
			OFile_MeanValues_real << real(p_mean(malla, n)) << "\t"; OFile_MeanValues_imag << imag(p_mean(malla, n)) << "\t";
		}
		OFile_MeanValues_real << endl; OFile_MeanValues_imag << endl;

		//Valor medio de p^2:
		for(int n = 0; n < N; ++n){
			OFile_MeanValues_real << real(p2_mean(malla, n)) << "\t"; OFile_MeanValues_imag << imag(p2_mean(malla, n)) << "\t";
		}
		OFile_MeanValues_real << endl; OFile_MeanValues_imag << endl;

		//Varianza de p:
		for(int n = 0; n < N; ++n){
			OFile_MeanValues_real << p_var(malla,n) << "\t"; OFile_MeanValues_imag << 0 << "\t";
		}
		OFile_MeanValues_real << endl; OFile_MeanValues_imag << endl;
		cout << "\nMomentos guardados";
	}


	cout << "\nDatos guardados :)";
    return 0;
}
