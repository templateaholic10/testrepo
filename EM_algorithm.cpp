char *gets(char *str);

#include <fstream>
#include "EM_algorithm.hpp"

template <class M>
double determinant(const M& m)
{
    namespace ublas = boost::numeric::ublas;

    BOOST_UBLAS_CHECK(m.size1() == m.size2(), ublas::external_logic());

    ublas::matrix<double> lu(m);
    ublas::permutation_matrix<> pm(m.size1());

    ublas::lu_factorize(lu, pm);

    double det(1);

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
double logL(const std::array<double, mixture_num>& pi, const double p[num][mixture_num])
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

namespace EM {

    template <int dim, int num>
    Data_series<dim, num>::Data_series(std::istream &datain)
    : _dim(dim), _num(num)
    {
        for (int t = 0; t < num; t++) {
            _x[t] = dvector<dim>();
            for (int i = 0; i < dim; i++) {
                datain >> _x[t](i);
            }
        }
    }

    template <int dim, int num, int mixture_num>
    EM_estimator<dim, num, mixture_num>::EM_estimator(std::istream &datain, std::istream &initialin)
    : _data(datain), _mixture_num(mixture_num)
    {
        std::string line;

        // 初期化
        _pi.clear();
        _mu.clear();
        _sigma.clear();
        _logL.clear();

        _step = 0;

        // 混合率_pi
        _pi.push_back(std::array<double, mixture_num>());
        for (int dist = 0; dist < mixture_num; dist++) {
            initialin >> _pi[_step][dist]; getline(initialin, line);
        }
        getline(initialin, line);

        // 平均_mu
        _mu.push_back(std::array<dvector<dim>, mixture_num>());
        for (int dist = 0; dist < mixture_num; dist++) {
            _mu[0][dist] = dvector<dim>();
            for (int i = 0; i < dim; i++) {
                initialin >> _mu[_step][dist](i);
            }
            getline(initialin, line);
        }
        getline(initialin, line);

        // 分散_sigma
        _sigma.push_back(std::array<dmatrix<dim>, mixture_num>());
        for (int dist = 0; dist < mixture_num; dist++) {
            _sigma[0][dist] = dmatrix<dim>();
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    initialin >> _sigma[_step][dist](i, j);
                }
            }
            getline(initialin, line);
        }
        getline(initialin, line);

        // 中間生成物p[t][dist]
        double p[num][mixture_num];
        dmatrix<dim> sigmaInverse;
        double sigmaDeterminant;
        for (int dist = 0; dist < mixture_num; dist++) {
            invert(_sigma[_step][dist], sigmaInverse);
            sigmaDeterminant = determinant(_sigma[_step][dist]);
            for (int t = 0; t < num; t++) {
                p[t][dist] = pnorm<dim>(_data._x[t], _mu[_step][dist], sigmaInverse, sigmaDeterminant);
            }
        }

        // 対数尤度_logL
        _logL.push_back(logL<num, mixture_num>(_pi[_step], p));
    }

    template <int dim, int num, int mixture_num>
    void EM_estimator<dim, num, mixture_num>::step()
    {
        // 定義式と同じくn+1←nの漸化式を計算する

        // 中間生成物p[t][dist]
        double p[num][mixture_num];
        dmatrix<dim> sigmaInverse;
        double sigmaDeterminant;
        for (int dist = 0; dist < mixture_num; dist++) {
            invert(_sigma[_step][dist], sigmaInverse);
            sigmaDeterminant = determinant(_sigma[_step][dist]);
            for (int t = 0; t < num; t++) {
                p[t][dist] = pnorm<dim>(_data._x[t], _mu[_step][dist], sigmaInverse, sigmaDeterminant);
            }
        }

        // 中間生成物gamma[t][dist]
        double gamma[num][mixture_num];
        double numerator, denominator;
        for (int dist = 0; dist < mixture_num; dist++) {

            denominator = 0.;
            for (int t = 0; t < num; t++) {
                denominator += _pi[_step][dist] * p[t][dist];
            }

            for (int t = 0; t < num; t++) {
                numerator = _pi[_step][dist] * p[t][dist];
                gamma[t][dist] = numerator / denominator;
            }
        }

        // 中間生成物gammaのデータに関する和
        double gammaSum[mixture_num];
        for (int dist = 0; dist < mixture_num; dist++) {
            gammaSum[dist] = 0.;
            for (int t = 0; t < num; t++) {
                gammaSum[dist] += gamma[t][dist];
            }
        }

        namespace ublas = boost::numeric::ublas;

        // 混合率_pi
        _pi.push_back(std::array<double, mixture_num>());
        for (int dist = 0; dist < mixture_num; dist++) {
            _pi[_step+1][dist] = gammaSum[dist] / num;
        }

        // 平均_mu
        _mu.push_back(std::array<dvector<dim>, mixture_num>());
        dvector<dim> cvector;
        for (int dist = 0; dist < mixture_num; dist++) {
            cvector = dvector<dim>();
            for (int t = 0; t < num; t++) {
                cvector += gamma[t][dist] * _data._x[t];
            }
            _mu[_step+1][dist] = cvector / gammaSum[dist];
        }

        // 分散_sigma
        _sigma.push_back(std::array<dmatrix<dim>, mixture_num>());
        dmatrix<dim> cmatrix;
        for (int dist = 0; dist < mixture_num; dist++) {
            cmatrix = dmatrix<dim>();
            for (int t = 0; t < num; t++) {
                cmatrix += gamma[t][dist] * ublas::outer_prod(_data._x[t], _data._x[t]);
            }
            _sigma[_step+1][dist] = cmatrix / gammaSum[dist] - ublas::outer_prod(_mu[_step+1][dist], _mu[_step+1][dist]);
        }

        // 対数尤度_logL
        _logL.push_back(logL<num, mixture_num>(_pi[_step], p));

        _step++;

        return;
    }

    template <int dim, int num, int mixture_num>
    bool EM_estimator<dim, num, mixture_num>::haltCheck() const
    {
        return (fabs(_logL[_step] - _logL[_step-1]) < _epsilon);
    }

    template <int dim, int num, int mixture_num>
    void EM_estimator<dim, num, mixture_num>::estimate()
    {
        _step = 0;

        do {
            step();
        } while (haltCheck());

        return;
    }

    template <int dim, int num, int mixture_num>
    void EM_estimator<dim, num, mixture_num>::output(std::ostream &estimateout, std::ostream &convergeout) const
    {
        for (int dist = 0; dist < mixture_num; dist++) {
            estimateout << _pi[_step][dist];
            for (int i = 0; i < dim; i++) {
                estimateout << " " << _mu[_step][dist](i);
            }
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    estimateout << " " << _sigma[_step][dist](i, j);
                }
            }
            estimateout << std::endl;
        }
        for (int i = 0; i <= _step; i++) {
            convergeout << _logL[i] << std::endl;
        }
    }
}

constexpr int DIM = 2;
constexpr int NUM = 100;
constexpr int MIXTURE_NUM = 2;

int main(int argc, char const *argv[]) {
    std::ifstream datain("data.txt");
    std::ifstream initialin("initial.txt");

    EM::EM_estimator<DIM, NUM, MIXTURE_NUM> em(datain, initialin);

    datain.close();
    initialin.close();

    em.estimate();

    std::ofstream estimateout("extimate.csv");
    std::ofstream logLout("logL.csv");

    em.output(estimateout, logLout);

    estimateout.close();
    logLout.close();

    return 0;
}
