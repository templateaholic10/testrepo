char* gets(char* str);

#include <complex>
#include <iostream>
using namespace std;
int main(){
	const complex<double> i( 0.0, 1.0 );
	complex<double> z1( -1.0, 1.0 );
	complex<double> z2( 2.0, -2.0 );
	complex<double> w = polar( 1.0, 3.141592 / 2.0 );//極座標
	//複素数の基本関数
	cout << std::real(z1) << " , "<< z1.imag() << "i"<<endl;
	cout << arg(z1) << endl;
	cout << norm(z1) << endl;
	cout << conj(z1)<<endl;
	//複素数の演算
	cout << z1 + 1.0 << endl;
	cout << 1.0 + z1 << endl;
	cout << z1 * i << endl;
	cout << (z1 == z2) << endl;
	cout << (z1 != z2) << endl;
	//複素関数
	cout << exp(w) << endl;
	cout << sin(w) << endl;
	cout << cos(w) << endl;
	cout << sinh(w) << endl;
	cout << cosh(w) << endl;
	cout << log(w) << endl;
	return 0;
}
