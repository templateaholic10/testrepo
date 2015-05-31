#ifndef DETAIL_STATISTIC_UTIL
#define DETAIL_STATISTIC_UTIL

#include "../statistic_util.hpp"

namespace statistic_util {

    template <int dim>
    double pnorm(const dvector<dim>& x, const dvector<dim>& mu, const dmatrix<dim>& sigmaInverse, double sigmaDeterminant)
    {
        namespace ublas = boost::numeric::ublas;

        double exponent(-0.5 * ublas::inner_prod((x - mu), ublas::prod(sigmaInverse, (x - mu))));

        return normalize<dim>() / sqrt(sigmaDeterminant) * exponent;
    }

    template <int dim>
    constexpr double normalize()
    {
        return pow(1/(sqrt(2. * M_PI)), dim);
    }

    template <int num, int mixture_num>
    double logL(const std::array<double, mixture_num>& pi, const std::array<std::array<double, mixture_num>, num>& p)
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
