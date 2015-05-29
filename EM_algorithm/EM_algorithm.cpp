char *gets(char *str);

#include <fstream>
#include "EM_algorithm.hpp"

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
        std::array<std::array<double, mixture_num>, num> p;
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
        std::array<std::array<double, mixture_num>, num> p;
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
