#include <iostream>
#include <complex>
#include <Eigen/Core>
#include <util>
#define COMPLEX_DEFAULT
#define EIGEN_MATRIX_DEFAULT
#include "default_value.hpp"

int main(int argc, char const *argv[])
{
    _DISPLAY(util::default_value<int>())
    _DISPLAY(util::default_value<double>())
    _DISPLAY(util::default_value<char>())
    _DISPLAY(util::default_value<float>())
    _DISPLAY(util::default_value<std::complex<double>>())
    _DISPLAY((util::default_value<Eigen::Matrix<int, 5, 1>>()))
    return 0;
}
