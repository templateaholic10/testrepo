#include <iostream>
#include <fstream>
#include <string>
#include <util>
#include <timer>
#include <randeigen>
#include <array>
#include <Eigen/QR>
#include "exeigen.hpp"
#include "eigen_io.hpp"

int main(int argc, char const *argv[])
{
    constexpr int n = 6;
    constexpr int max_rep = 100;
    using m_type = Eigen::Matrix<double, n, n>;
    m_type M;
    M = Eigen::rand_uniform<double, n, n>(0., 10.);
    _DISPLAY(M)
    m_type Q = Eigen::householderQ_unique(M);
    m_type R = Q.transpose() * M;
    _DISPLAY(R)
    return 0;
}
