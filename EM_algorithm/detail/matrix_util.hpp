#ifndef DETAIL_MATRIX_UTIL
#define DETAIL_MATRIX_UTIL

#include "../matrix_util.hpp"

namespace matrix_util {

    template <class M>
    double determinant(const M& m)
    {
        namespace ublas = boost::numeric::ublas;

        BOOST_UBLAS_CHECK(m.size1() == m.size2(), ublas::external_logic());

        ublas::matrix<double> lu(m);
        ublas::permutation_matrix<> pm(m.size1());

        ublas::lu_factorize(lu, pm);

        double det(1.);

        using size_type = ublas::permutation_matrix<>::size_type;

        for (size_type i = 0; i < pm.size(); ++i) {
            det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
        }

        return det;
    }

    template <class M, class MI>
    void invert(const M& m, MI& mi)
    {
        namespace ublas = boost::numeric::ublas;

        BOOST_UBLAS_CHECK(m.size1() == m.size2(), ublas::external_logic());

        ublas::matrix<double> lhs(m);
        ublas::matrix<double> rhs(ublas::identity_matrix<double>(m.size1()));
        ublas::permutation_matrix<> pm(m.size1());

        ublas::lu_factorize(lhs, pm);
        ublas::lu_substitute(lhs, pm, rhs);

        BOOST_UBLAS_CHECK(rhs.size1() == m.size1(), ublas::internal_logic());
        BOOST_UBLAS_CHECK(rhs.size2() == m.size2(), ublas::internal_logic());

    #if BOOST_UBLAS_TYPE_CHECK
        BOOST_UBLAS_CHECK(
            ublas::detail::expression_type_check(
                ublas::prod(m, rhs),
                ublas::identity_matrix<typename M::value_type>(m.size1())
            ),
            ublas::internal_logic()
        );
    #endif

        mi.resize(rhs.size1(), rhs.size2(), false);
        mi.assign(rhs);
        // mi.assign_temporary(rhs);
    }

}

#endif
