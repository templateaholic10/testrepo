#ifndef DETAIL_MATRIX_UTIL
#define DETAIL_MATRIX_UTIL

#include "../matrix_util.hpp"

namespace matrix_util {
    using namespace boost::numeric;

    template <class M>
    boost::optional<double> determinant(const M &m)
    {
        namespace ublas = boost::numeric::ublas;

        // 正方行列でないとき，失敗．
        if (m.size1() != m.size2()) {
            return boost::none;
        }

        ublas::matrix <double>       lu(m);
        ublas::permutation_matrix <> pm(m.size1());

        ublas::lu_factorize(lu, pm);

        double det(1.);

        using size_type = ublas::permutation_matrix <>::size_type;

        for (size_type i = 0; i < pm.size(); ++i) {
            det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
        }

        return det;
    }

    template <class M>
    boost::optional<M> invert(const M &m)
    {
        namespace ublas = boost::numeric::ublas;

        // 正方行列でないとき，失敗．
        if (m.size1() != m.size2()) {
            return boost::none;
        }

        ublas::matrix <double>       lhs(m);
        ublas::matrix <double>       rhs(ublas::identity_matrix <double>(m.size1()));
        ublas::permutation_matrix <> pm(m.size1());

        // LU分解可能でないとき，失敗．
        if (ublas::lu_factorize(lhs, pm) != 0) {
            return boost::none;
        }

        ublas::lu_substitute(lhs, pm, rhs);

        if (rhs.size1() != m.size1() || rhs.size2() != m.size2()) {
            return boost::none;
        }

        // #if BOOST_UBLAS_TYPE_CHECK
        //     BOOST_UBLAS_CHECK(
        //         ublas::detail::expression_type_check(
        //             ublas::prod(m, rhs),
        //             ublas::identity_matrix <typename M::value_type>(m.size1())
        //             ),
        //         ublas::internal_logic()
        //         );
        // #endif

        // >>>>>>>>>>
        /*
        mi.resize(rhs.size1(), rhs.size2(), false);
        mi.assign(rhs);
        // mi.assign_temporary(rhs);
        */

        // ----------
        return std::move(rhs);
        // <<<<<<<<<<
    }

    template <class M>
    boost::optional<M> transToSigma(const M &m)
    {
        // boostのsize_typeはstd::size_t
        static_assert(std::is_same <decltype(ublas::vector <double>(1).size()), std::size_t>::value, "boost size_type check");

        // 正方行列でないとき，失敗．
        if (m.size1() != m.size2()) {
            return boost::none;
        }

        return std::move(boost::numeric::ublas::prod(m, boost::numeric::ublas::trans(m)));
    }

    // std::arrayからbounded_vectorを作る
    template <typename T, std::size_t dim>
    ublas::bounded_vector <T, dim> make_bounded_vector(const std::array <T, dim> &v)
    {
        ublas::bounded_vector <T, dim> bv;
        std::copy(v.begin(), v.end(), bv.begin());

        return std::move(bv);
    }

    // std::array<std::array<T, n2>, n1>からbounded_matrixを作る
    template <typename T, std::size_t dim1, std::size_t dim2>
    ublas::bounded_matrix <T, dim1, dim2> make_bounded_matrix(const std::array <std::array <T, dim2>, dim1> &M)
    {
        ublas::bounded_matrix <T, dim1, dim2> bm;
        for (int i = 0; i < dim1; i++) {
            for (int j = 0; j < dim2; j++) {
                bm(i, j) = M[i][j];
            }
        }

        return std::move(bm);
    }

    // ベクトルのL-inf距離
    template <typename T, std::size_t dim>
    double d_inf(const ublas::bounded_vector <T, dim> &v1, const ublas::bounded_vector <T, dim> &v2)
    {
        auto v = v2 - v1;
        T max = 0.;
        for (size_t i = 0; i < dim; i++) {
            if (fabs(v(i)) > max) {
                max = fabs(v(i));
            }
        }
        return max;
    }

    // 行列のL-inf距離
    template <typename T, std::size_t dim1, std::size_t dim2>
    double d_inf(const ublas::bounded_matrix <T, dim1, dim2> &m1, const ublas::bounded_matrix <T, dim1, dim2> &m2)
    {
        auto m = m2 - m1;
        T max = 0.;
        for (size_t i = 0; i < dim1; i++) {
            for (size_t j = 0; j < dim2; j++) {
                if (fabs(m(i, j)) > max) {
                    max = fabs(m(i, j));
                }
            }
        }
        return max;
    }
}

#endif
