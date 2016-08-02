#include <exeigen>
#include <eigen_io>
#include "../lab/eigensparse_io.hpp"
#include <Eigen/Sparse>
#include <iostream>
#include <fstream>

int main()
{
    using M_type = Eigen::SparseMatrix<double>;
    M_type M(4, 5);
    // M.setIdentity();
    std::ifstream fin("M.mat");
    in(fin, M);
    fin.close();
    std::cout << M << std::endl;
    // out(std::cout, M) << std::endl;
    // M_type N(5, 4);
    // N = Eigen::Map<M_type>(5, 4, M.nonZeros(), M.outerIndexPtr(), M.innerIndexPtr(), M.valuePtr());
    // std::cout << N << std::endl;
}
