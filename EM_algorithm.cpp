#include <fstream>
#include "EM_algorithm.hpp"

namespace EM {

    template <int dim, int num>
    Data_series<dim, num>::Data_series(std::istream &datain)
    : _dim(dim), _num(num)
    {
        for (int dataid = 0; dataid < num; dataid++) {
            for (int i = 0; i < dim; i++) {
                datain >> _x[dataid][i];
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
        _likelihood.clear();

        _step = 0;

        // 混合率_pi
        _pi.push_back(std::array<double, mixture_num>());
        for (int dist = 0; dist < mixture_num; dist++) {
            initialin >> _pi[0][dist]; getline(initialin, line);
        }
        getline(initialin, line);

        // 平均_mu
        _mu.push_back(std::array<dvector<dim>, mixture_num>());
        for (int dist = 0; dist < mixture_num; dist++) {
            _mu[0][dist] = dvector<dim>();
            for (int i = 0; i < dim; i++) {
                initialin >> _mu[0][dist](i);
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
                    initialin >> _sigma[0][dist](i, j);
                }
            }
            getline(initialin, line);
        }
        getline(initialin, line);

        // 対数尤度の期待値_E_logL
        
    }

    template <int dim, int num, int mixture_num>
    void EM_estimator<dim, num, mixture_num>::step()
    {
        // 定義式と同じくn+1←nの漸化式を計算する

        // 中間生成物gamma
        dvector<dim> gamma[mixture_num];
        double upper, lower;
        for (int dist = 0; dist < num; dist++) {
            upper = _pi[step][dist]
        }

        // 混合率_pi

        // 平均_mu

        // 分散_sigma

        // 対数尤度の期待値

        _step++;

        return;
    }

    template <int dim, int num, int mixture_num>
    void EM_estimator<dim, num, mixture_num>::haltCheck()
    {
        return;
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

    do {
        /* code */
    } while (!em.haltCheck());

    std::ofstream fout("result.csv");

    em.output(fout);

    fout.close();

    return 0;
}
