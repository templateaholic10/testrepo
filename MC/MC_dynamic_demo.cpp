#include <iostream>
#include <fstream>
#include <eigen_io>
#include "../lab/eigensparse_io.hpp"
#include <debug>
#include "MC_dynamic.hpp"

constexpr int m = 4;
constexpr int n = 5;
using Scalar = double;
using M_type = Eigen::MatrixX<Scalar>;
using SM_type = Eigen::SMatrix<Scalar>;
using rM_type = Eigen::MatrixX<Scalar>;
using cM_type = Eigen::MatrixX<Scalar>;
using bigV_type = Eigen::VectorX<Scalar>;
using bigM_type = Eigen::MatrixX<Scalar>;

const std::string M_filename = "M.mat";
const std::string Lr_filename = "Lr.mat";
const std::string Lc_filename = "Lc.mat";
const std::string prob_param_filename = "prob_param";

int main()
{
    MC<Scalar> mc(m, n);

    std::ifstream fin(M_filename);
    in(fin, mc.M);
    fin.close();

    // mc.M = (mc.M.array() - mc.M.mean()).matrix();
    mc.A = make_mask(mc.M);
    mc.Lr.setZero();
    mc.Lc.setZero();

    fin.open(prob_param_filename);
    fin >> mc.gamma_n >> mc.gamma_r >> mc.gamma_c >> mc.rho;
    fin.close();

    mc.go();
    _PRINT(M_type(mc.M))
    _PRINT(mc.X)
    return 0;
}
