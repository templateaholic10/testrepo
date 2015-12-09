#include <iostream>
#include <fstream>
#include <string>
#include <util>
#include <timer>
#include "randeigen.hpp"
#include <array>
#include <Eigen/QR>
#include "exeigen.hpp"
#include "eigen_io.hpp"
#include "array_io.hpp"
#include "exfstream.hpp"

template <typename T, size_t n>
void hoge(std::array<T, n>& a)
{
    for (size_t i = 0; i < n; i++) {
        a[i] = 1.;
    }
}

template <typename T, size_t n>
void piyo(std::array<T, n>& a)
{
    for (auto it = a.begin(); it != a.end(); it++) {
        (*it) = 2.;
    }
}

template <typename T, size_t n>
void fuga(std::array<T, n>& a)
{
    for (auto elem : a) {
        elem = 3.;
    }
}

template <typename T, size_t n>
void moge(std::array<T, n>& a)
{
    for (auto &&elem : a) {
        elem = 4.;
    }
}

int main(int argc, char const *argv[])
{
    const std::string filename = "gaussians.csv";
    constexpr int n = 2;
    constexpr int max_rep = 10;
    using v_type = Eigen::RowVector<double, n>;
    using matrix_type = Eigen::Matrix<double, n, n>;
    std::array<v_type, max_rep> v;
    std::array<double, max_rep> a;
    // v_type mu;
    // mu << 1., 1.;
    // matrix_type sigma;
    // sigma << 1., 0.9, 0.9, 1.;
    // std::Gaussian<v_type> gaussian(mu, sigma);
    // for (int i = 0; i < max_rep; i++) {
    //     v[i] = gaussian();
    //     dump(filename, v[i], std::ios_base::app);
    // }
    // std::cout << v << std::endl;
    // std::cin >> v[0];
    // std::cin >> v;
    load(filename, v);
    std::cout << v << std::endl;

    return 0;
}
