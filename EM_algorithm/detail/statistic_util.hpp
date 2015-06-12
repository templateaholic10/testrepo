#ifndef DETAIL_STATISTIC_UTIL
#define DETAIL_STATISTIC_UTIL

#include "../statistic_util.hpp"

namespace statistic_util {
    template <int dim>
    double determinant(const dmatrix <dim> &m)
    {
        namespace ublas = boost::numeric::ublas;

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

    template <int dim>
    boost::optional <dmatrix <dim> > invert(const dmatrix <dim> &m)
    {
        namespace ublas = boost::numeric::ublas;

        dmatrix <dim>                lhs(m);
        dmatrix <dim>                rhs(ublas::identity_matrix <double>(m.size1()));
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

    template <int dim>
    boost::optional <dmatrix <dim> > invert(dmatrix <dim> &m)
    {
        namespace ublas = boost::numeric::ublas;

        dmatrix <dim>                lhs(m);
        dmatrix <dim>                rhs(ublas::identity_matrix <double>(m.size1()));
        ublas::permutation_matrix <> pm(m.size1());

        // LU分解可能でないとき，失敗．
        if (ublas::lu_factorize(lhs, pm) != 0) {
            return boost::none;
        }

        // ここでも落ちる？
        try {
            ublas::lu_substitute(lhs, pm, rhs);
        }
        catch (...) {
            return boost::none;
        }

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

    template <int dim>
    dmatrix <dim> transToSigma(const dmatrix <dim> &m)
    {
        return std::move(boost::numeric::ublas::prod(m, boost::numeric::ublas::trans(m)));
    }

    template <int dim>
    double pnorm(const dvector <dim> &x, const dvector <dim> &mu, const dmatrix <dim> &sigmaInverse, double sigmaDeterminant)
    {
        namespace ublas = boost::numeric::ublas;

        double exponent = -0.5 * ublas::inner_prod((x - mu), ublas::prod(sigmaInverse, (x - mu)));

        return normalize <dim>() / sqrt(sigmaDeterminant) * exp(exponent);
    }

    template <int dim>
    constexpr double normalize()
    {
        return pow(1 / (sqrt(2. * M_PI)), dim);
    }

    template <int num, int mixture_num>
    double logL(const std::array <double, mixture_num> &pi, const std::array <std::array <double, mixture_num>, num> &p)
    {
        double logL_all(0.);
        double L_of_data;

        for (int t = 0; t < num; t++) {
            L_of_data = 0.;
            for (int dist = 0; dist < mixture_num; dist++) {
                L_of_data += pi[dist] * p[t][dist];
            }
            logL_all += log(L_of_data);
        }

        return logL_all;
    }
}

#endif
