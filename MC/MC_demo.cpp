#include <iostream>
#include <debug>
#include "MC.hpp"

int main()
{
    constexpr int m = 5;
    constexpr int n = 4;
    using Scalar = double;
    using M_type = Eigen::Matrix<Scalar, m, n>;
    using rM_type = Eigen::Matrix<Scalar, m, m>;
    using cM_type = Eigen::Matrix<Scalar, n, n>;
    MC<m, n> mc;
    mc.M << 1,0,3,0,2,3,0,2,0,0,2,0,2,1,0,3,0,3,5,2;
    mc.M = (mc.M.array() - mc.M.mean()).matrix();
    mc.A = make_mask(mc.M);
    _PRINT(mc.A)
    mc.Lr = rM_type::Zero();
    mc.Lc = cM_type::Zero();
    mc.gamma_n = 1.0;
    mc.gamma_r = 0.0;
    mc.gamma_c = 0.0;
    mc.rho = 0.15;
    mc.go();
    _PRINT(mc.M)
    _PRINT(mc.X)
    return 0;
}
