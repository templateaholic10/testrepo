#include <iostream>
#include "DAD.hpp"
#include <debug>

int main(int argc, char const *argv[])
{
    using scalar_type = std::complex<double>;
    using scalarR_type = typename std::decomplexify<scalar_type>::type;
    constexpr int dim = 2;

    using vector_type = Eigen::Vector<scalar_type, dim>;
    using vectorR_type = Eigen::Vector<scalarR_type, dim>;
    using matrix_type = Eigen::Matrix<scalar_type, dim, dim>;

    matrix_type A(2. * matrix_type::Identity());
    vectorR_type a(vectorR_type::Ones());
    _PRINT(A)
    _PRINT(a)

    linear_algebra::DAD<matrix_type> dad(A, a);
    vector_type x = dad.solve();
    _PRINT(x)

    return 0;
}
