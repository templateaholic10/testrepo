char *gets(char *str);

#include <iostream>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>         // (1) 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp>     // (2) 三角行列用のヘッダ
#include <boost/numeric/ublas/symmetric.hpp>      // (3) 対称行列用のヘッダ
#include <boost/numeric/ublas/hermitian.hpp>      // (4) エルミート行列用のヘッダ
#include <boost/numeric/ublas/matrix_sparse.hpp>  // (5) 疎行列用のヘッダ
#include <boost/numeric/ublas/vector.hpp>         // (6) ベクトル用のヘッダ
#include <boost/numeric/ublas/vector_sparse.hpp>  // (7) 疎ベクトル用のヘッダ
using dvector = boost::numeric::ublas::vector<double>;
using dmatrix = boost::numeric::ublas::matrix<double>;

namespace EM {

    constexpr int DIM = 2;
    constexpr int NUM = 100;

    template <int dim, int num> struct Data_series;
    template <int dim, int num, int mixture_num> class EM_estimator;

    template <int dim, int num>
    struct Data_series
    {
        void input(std::ostream &os);

        int _dim;
        int _num;
        dvector _x[num];  // 次元はdim，要素数はnum
    };

    template <int dim, int num, int mixture_num>
    class EM_estimator
    {
    public:
        void step();
        void haltCheck();
    private:
        int _step;
        int _epsilon;

        Data_series<dim, num> _data;
        std::vector<double[mixture_num]> _pi;  // ステップ数，分布番号
        std::vector<dvector[mixture_num]> _mu;  // ステップ数，分布番号，次元
        std::vector<dmatrix[mixture_num]> _sigma;  // ステップ数，分布番号，次元
        std::vector<double> _likelihood;  // 対数尤度の期待値
    };

}
