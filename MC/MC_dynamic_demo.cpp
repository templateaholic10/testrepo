#include <iostream>
#include <fstream>
#include <eigen_io>
#include <eigensparse_io>
#include <debug>
#include <os>
#include "MC_dynamic.hpp"

using Scalar = double;
using M_type = Eigen::MatrixX<Scalar>;
using SM_type = Eigen::SMatrix<Scalar>;
using rM_type = Eigen::MatrixX<Scalar>;
using cM_type = Eigen::MatrixX<Scalar>;
using bigV_type = Eigen::VectorX<Scalar>;
using bigM_type = Eigen::MatrixX<Scalar>;

/*! @brief this m n project
*/
int main(int argc, char *argv[])
{
    if (argc < 4) {
        std::cerr << "MC_dynamic_demo [m] [n] [project] [(option)log directory in project]" << std::endl;
        return 1;
    }
    const int m = atoi(argv[1]);
    const int n = atoi(argv[2]);
    const std::string project = argv[3];
    const std::string log_dirname = (argc == 5) ? os::path::join(project, argv[4]) : os::path::join(project, "log");

    const std::string M_filename = os::path::join(project, "M.mat");
    const std::string Lr_filename = os::path::join(project, "Lr.mat");
    const std::string Lc_filename = os::path::join(project, "Lc.mat");
    const std::string prob_param_filename = "prob_param";
    const std::string solver_param_filename = "solver_param";
    const std::string X_filename = os::path::join(project, "X.mat");

    MC<Scalar> mc(m, n);

    std::ifstream fin(M_filename);
    in(fin, mc.M);
    fin.close();

    // mc.M = (mc.M.array() - mc.M.mean()).matrix();
    mc.A = make_mask(mc.M);

    fin.open(Lr_filename);
    in(fin, mc.Lr);
    fin.close();

    fin.open(Lc_filename);
    in(fin, mc.Lc);
    fin.close();

    fin.open(prob_param_filename);
    fin >> mc.gamma_n >> mc.gamma_r >> mc.gamma_c >> mc.rho;
    fin.close();

    fin.open(solver_param_filename);
    fin >> MC<Scalar>::max_rep >> MC<Scalar>::abs_tol >> MC<Scalar>::rel_tol;
    fin.close();

    MC<Scalar>::logfile = os::path::join(log_dirname, "X");

    mc.go();

    std::ofstream fout(X_filename);
    out(fout, mc.X);
    fout.close();

    return 0;
}
