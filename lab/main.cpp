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
    using T = std::complex<double>;
    constexpr int N = 5;
    constexpr int p = 3;
    using v_type = std::array<T, N>;
    // using M_type = Eigen::Matrix<T, N, p>;
    v_type v;
    // M_type M;
    // std::iota(v.begin(), v.end(), 0.);
    // M = M_type::Identity();
    _PRINT(v)
    // auto V = Eigen::dct1<T>(v);
    // _PRINT(V)
    std::Elemwise<v_type> hoge = std::Elemwise<v_type>(v);
    _PRINT(hoge)
    auto piyo = hoge;
    _PRINT(hoge)
    _PRINT(piyo)
    hoge[2] += 5;
    _PRINT(hoge)
    _PRINT(piyo)
    // _PRINT((hoge >= piyo))
    // _PRINT((piyo >= hoge))
    hoge = std::Elemwise<v_type>(v);
    _PRINT(hoge.real())
    v_type w(hoge.array());
    _PRINT(w)
    // _PRINT(Eigen::deevenize1<Eigen::RowMajor>(Meven))

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
