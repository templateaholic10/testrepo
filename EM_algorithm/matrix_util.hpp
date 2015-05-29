#ifndef MATRIX_UTIL
#define MATRIX_UTIL

#include <boost/numeric/ublas/matrix.hpp>         // (1) 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp>     // (2) 三角行列用のヘッダ
#include <boost/numeric/ublas/symmetric.hpp>      // (3) 対称行列用のヘッダ
#include <boost/numeric/ublas/hermitian.hpp>      // (4) エルミート行列用のヘッダ
#include <boost/numeric/ublas/matrix_sparse.hpp>  // (5) 疎行列用のヘッダ
#include <boost/numeric/ublas/vector.hpp>         // (6) ベクトル用のヘッダ
#include <boost/numeric/ublas/vector_sparse.hpp>  // (7) 疎ベクトル用のヘッダ
#include <boost/numeric/ublas/lu.hpp>

namespace matrix_util {

    // 行列式
    template <class M>
    double determinant(const M& m);

    // 逆行列
    template <class M, class MI>
    void invert(const M& m, MI& mi);

}

#include "detail/matrix_util.hpp"

#endif
