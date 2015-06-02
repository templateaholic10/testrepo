#ifndef MATRIX_UTIL
#define MATRIX_UTIL

#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>         // (1) 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp>     // (2) 三角行列用のヘッダ
#include <boost/numeric/ublas/symmetric.hpp>      // (3) 対称行列用のヘッダ
#include <boost/numeric/ublas/hermitian.hpp>      // (4) エルミート行列用のヘッダ
#include <boost/numeric/ublas/matrix_sparse.hpp>  // (5) 疎行列用のヘッダ
#include <boost/numeric/ublas/vector.hpp>         // (6) ベクトル用のヘッダ
#include <boost/numeric/ublas/vector_sparse.hpp>  // (7) 疎ベクトル用のヘッダ
#include <boost/numeric/ublas/lu.hpp>

namespace matrix_util {
    using namespace boost::numeric;

    // 行列式
    template <class M>
    double determinant(const M &m);

    // 逆行列
    template <class M>
    M invert(const M &m);

    // 座標変換行列から分散行列を得る
    template <class M>
    M transToSigma(const M &m);

    // std::arrayからbounded_vectorを作る
    template <typename T, std::size_t dim>
    ublas::bounded_vector <T, dim> make_bounded_vector(const std::array <T, dim> &v);

    // std::array<std::array<T, n2>, n1>からbounded_matrixを作る
    template <typename T, std::size_t dim1, std::size_t dim2>
    ublas::bounded_matrix <T, dim1, dim2> make_bounded_matrix(const std::array <std::array <T, dim2>, dim1> &M);

    // ベクトルのL-inf距離
    template <typename T, std::size_t dim>
    double d_inf(const ublas::bounded_vector <T, dim>& v1, const ublas::bounded_vector <T, dim>& v2);

    // 行列のL-inf距離
    template <typename T, std::size_t dim1, std::size_t dim2>
    double d_inf(const ublas::bounded_matrix <T, dim1, dim2>& m1, const ublas::bounded_matrix <T, dim1, dim2>& m2);
}

#include "detail/matrix_util.hpp"

#endif
