#include "exeigen.hpp"
#include <iostream>
#include <complex>
#include "debug.hpp"

int main()
{
    using M_type = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
    M_type M(M_type::Identity(3, 4));
    _PRINT(M)
    // _PRINT((M.block<2, 2>(0, 0, 2, 2)))
    _PRINT(Eigen::quadjoin(M.block(0, 0, 2, 3), M.block(0, 3, 2, 1), M.block(2, 0, 1, 3), M.block(2, 3, 1, 1)))
}
