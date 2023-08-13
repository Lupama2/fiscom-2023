/*
Test de n�meros complejos:
https://en.cppreference.com/w/cpp/numeric/complex

*/


#include <iostream>

#include <iomanip>
#include <complex>
#include <cmath>


using namespace std;
//using namespace std::complex_literals;
int main()
{
    complex<float> x[2];
    x[0] = 1i;
    x[1] = 2i;
    complex<float> z = 4;
    cout << exp(x[0]) << endl;

    return 0;
}
