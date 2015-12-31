#include <iostream>
#include "exeigen.hpp"
#include "excomplex.hpp"
#include "randeigen.hpp"
#include "dct.hpp"
#include <eigen_io>
#include <array_io>
#include <vector_io>
#include "debug.hpp"
#include <numeric>
#include "elemwise.hpp"

int main(int argc, char const *argv[])
{
    using T = double;
    constexpr int N = 3;
    constexpr int p = 3;

    // arrayのテスト
    // using v_type = std::array<T, N>;
    // v_type v;
    // v.fill(0.);
    // v[0] = 1.;
    // _PRINT(v)
    // auto V = Eigen::dct2(v);
    // _PRINT(V)
    // auto v2 = Eigen::idct2(V);
    // _PRINT(v2)

    // 行列のテスト
    using M_type = Eigen::Matrix<T, N, p>;
    M_type M;
    M = M_type::Identity();
    _PRINT(M)
    _PRINT(Eigen::fft(M, Eigen::ColMajor))
    _PRINT(Eigen::fft(M, Eigen::RowMajor))
    // auto Mf = Eigen::dct2<Eigen::RowMajor>(M);
    // _PRINT(Mf)
    // auto M2 = Eigen::idct2<Eigen::RowMajor>(Mf);
    // _PRINT(M2)

    // _PRINT((Eigen::Vector<std::complex<T>, N>::Ones()))

    // Eigen::Matrix<T, N, p> x;
    // // 正弦波でテストする
    // constexpr double period = 2. * M_PI;
    // constexpr double interval = period / N;
    // for (size_t i = 0; i < N; i++) {
    //     x(i, 0) = std::sin(i * interval);
    //     x(i, 1) = std::cos(i * interval);
    // }
    // _PRINT(x)
    // auto X1 = Eigen::fft<T>(x);
    // _PRINT(X1)
    // auto X2 = Eigen::fft<std::complex<T>>(x);
    // _PRINT(X2)
    // auto X3 = Eigen::fft(x);
    // _PRINT(X3)
    // auto X4 = Eigen::ifft<T>(x);
    // _PRINT(X4)
    // auto X5 = Eigen::ifft<std::complex<T>>(x);
    // _PRINT(X5)
    // auto X6 = Eigen::ifft(x);
    // _PRINT(X6)
    // auto y = Eigen::ifft<std::complex<T>>(X);
    // _PRINT(y)
    return 0;
}
