char *gets(char *str);

#include <iostream>
#include <vector>
#include <array>
#include <boost/numeric/ublas/matrix.hpp>         // (1) 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp>     // (2) 三角行列用のヘッダ
#include <boost/numeric/ublas/symmetric.hpp>      // (3) 対称行列用のヘッダ
#include <boost/numeric/ublas/hermitian.hpp>      // (4) エルミート行列用のヘッダ
#include <boost/numeric/ublas/matrix_sparse.hpp>  // (5) 疎行列用のヘッダ
#include <boost/numeric/ublas/vector.hpp>         // (6) ベクトル用のヘッダ
#include <boost/numeric/ublas/vector_sparse.hpp>  // (7) 疎ベクトル用のヘッダ

// エイリアステンプレート
template <int size>
using dvector = boost::numeric::ublas::bounded_vector<double, size>;

template <int size>
using dmatrix = boost::numeric::ublas::bounded_matrix<double, size, size>;

namespace EM {

    template <int dim, int num> struct Data_series;
    template <int dim, int num, int mixture_num> class EM_estimator;

    template <int dim, int num>
    struct Data_series
    {
        Data_series() = delete;
        Data_series(std::istream &datain);

        int _dim;
        int _num;
        double _x[num][dim];  // 次元はdim，要素数はnum
    };

    template <int dim, int num, int mixture_num>
    class EM_estimator
    {
    public:
        EM_estimator() = delete;
        EM_estimator(std::istream &datain, std::istream &initialin);
        ~EM_estimator() = default;

        void step();
        bool haltCheck() const;

        void output(std::ostream &os) const;

    private:
        int _mixture_num;
        int _step;
        static constexpr double _epsilon = 0.01;

        Data_series<dim, num> _data;
        std::vector<std::array<double, mixture_num>> _pi;  // _pi[step][dist]：ステップ数，分布番号
        std::vector<std::array<dvector<dim>, mixture_num>> _mu;  // _mu[step][dist](i)：ステップ数，分布番号，次元
        std::vector<std::array<dmatrix<dim>, mixture_num>> _sigma;  // _sigma[step][dist](i, j)：ステップ数，分布番号，次元
        std::vector<double> _E_logL;  // _E_logL[step]：対数尤度の期待値
    };

}
