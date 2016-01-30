/*! @file
    @brief Eigenでランダム行列を扱うためのユーティリティ関数群
    @author yanteyon10
    @date 11/21
*/
#ifndef RANDEIGEN_HPP
#define RANDEIGEN_HPP

#include <exeigen>
#include <exrandom>
#include <Eigen/Cholesky>

namespace std {
    /*! @class
        @brief 標準ガウシアン関数オブジェクト
        @tparam T double
    */
    template <typename T, class Ignored = void>
    class Standard_gaussian;

    template <typename T, int n>
    class Standard_gaussian <Eigen::Vector<T, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using type = Standard_gaussian<Eigen::Vector<T, n>>;
        using result_type = Eigen::Vector<T, n>;
    private:
        Gaussian<T> gaussian;
    public:
        const int size;
    public:
        Standard_gaussian(const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : gaussian(0., 1., seed), size(size_)
        {
        }

        result_type operator()()
        {
            result_type retval(size);
            for (int i = 0; i < size; i++) {
                retval(i) = gaussian();
            }
            return retval;
        }

        result_type mean() const
        {
            return result_type::Zero(size);
        }

        result_type variance() const
        {
            return result_type::Identity(size, size);
        }
    };

    template <typename T, int n>
    class Standard_gaussian <Eigen::RowVector<T, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using type = Standard_gaussian<Eigen::RowVector<T, n>>;
        using result_type = Eigen::RowVector<T, n>;
    private:
        Gaussian<T> gaussian;
    public:
        const int size;
    public:
        Standard_gaussian(const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : gaussian(0., 1., seed), size(size_)
        {
        }

        result_type operator()()
        {
            result_type retval(size);
            for (int i = 0; i < size; i++) {
                retval(i) = gaussian();
            }
            return retval;
        }

        result_type mean() const
        {
            return result_type::Zero(size);
        }

        result_type variance() const
        {
            return result_type::Identity(size, size);
        }
    };

    /*! @brief std::GaussianのEigen::Vector版
    */
    template <typename T, int n>
    class Gaussian <Eigen::Vector <T, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> : public Standard_gaussian <Eigen::Vector <T, n>> {
    public:
        using type = Gaussian<Eigen::Vector<T, n>>;
        using base_type = Standard_gaussian <Eigen::Vector <T, n>>;
        using result_type = Eigen::Vector <T, n>;
        using matrix_type = Eigen::Matrix<T, n, n>;
    private:
        const result_type mu;
        const matrix_type A;
    public:
        Gaussian(const result_type& mu_=result_type::Zero(), const matrix_type& sigma_=matrix_type::Identity(), const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : base_type(seed, size_), mu(mu_), A(sigma_.llt().matrixL())
        {
        }

        result_type operator()()
        {
            return mu + A * base_type::operator()();
        }

        result_type mean() const
        {
            return mu;
        }

        result_type variance() const
        {
            return A * A.transpose();
        }
    };

    /*! @brief std::GaussianのEigen::RowVector版
    */
    template <typename T, int n>
    class Gaussian <Eigen::RowVector <T, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> : public Standard_gaussian <Eigen::RowVector <T, n>> {
    public:
        using type = Gaussian<Eigen::RowVector<T, n>>;
        using base_type = Standard_gaussian <Eigen::RowVector <T, n>>;
        using result_type = Eigen::RowVector <T, n>;
        using matrix_type = Eigen::Matrix<T, n, n>;
    private:
        const result_type mu;
        const matrix_type A;
    public:
        Gaussian(const result_type& mu_=result_type::Zero(), const matrix_type& sigma_=matrix_type::Identity(), const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : base_type(seed, size_), mu(mu_), A(sigma_.llt().matrixU())
        {
        }

        result_type operator()()
        {
            return mu + base_type::operator()() * A;
        }

        result_type mean() const
        {
            return mu;
        }

        result_type variance() const
        {
            return A.transpose() * A;
        }
    };

    namespace {
        constexpr int doubled_size(const int size)
        {
            if (size == Eigen::Dynamic) {
                return Eigen::Dynamic;
            } else {
                return 2*size;
            }
        }
    }

    /*! @brief std::Gaussianのstd::complex，Eigen::Vector版．ただしcircularly symmetricな場合
    */
    template <typename T, int n>
    class Gaussian <Eigen::Vector <std::complex<T>, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> : public Gaussian <Eigen::Vector <T, doubled_size(n)>> {
    public:
        using type = Gaussian<Eigen::Vector<std::complex<T>, n>>;
        using base_type = Gaussian <Eigen::Vector <T, doubled_size(n)>>;
        using result_type = Eigen::Vector <std::complex<T>, n>;
        using real_type = Eigen::Vector<T, doubled_size(n)>;
        using matrix_type = Eigen::Matrix<std::complex<T>, n, n>;
    private:
        const result_type mu;
        const matrix_type sigma;
        const int size;
    public:
        Gaussian(const result_type& mu_=result_type::Zero(), const matrix_type& sigma_=matrix_type::Identity(), const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : base_type(Eigen::vjoin(mu_.real(), mu_.imag()), 0.5*Eigen::quadjoin(sigma_.real(), -sigma_.imag(), sigma_.imag(), sigma_.real()), seed, 2*size_), mu(mu_), sigma(sigma_), size(size_)
        {
        }

        result_type operator()()
        {
            real_type tmp = base_type::operator()();
            result_type retval(size);
            for (int i = 0; i < size; i++) {
                retval(i).real(tmp(i));
                retval(i).imag(tmp(size+i));
            }
            return retval;
        }

        result_type mean() const
        {
            return mu;
        }

        result_type variance() const
        {
            return sigma;
        }
    };

    /*! @brief std::Gaussianのstd::complex，Eigen::RowVector版．ただしcircularly symmetricな場合
    */
    template <typename T, int n>
    class Gaussian <Eigen::RowVector <std::complex<T>, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> : public Gaussian <Eigen::RowVector <T, doubled_size(n)>> {
    public:
        using type = Gaussian<Eigen::RowVector<std::complex<T>, n>>;
        using base_type = Gaussian <Eigen::RowVector <T, doubled_size(n)>>;
        using result_type = Eigen::RowVector <std::complex<T>, n>;
        using real_type = Eigen::RowVector<T, doubled_size(n)>;
        using matrix_type = Eigen::Matrix<std::complex<T>, n, n>;
    private:
        const result_type mu;
        const matrix_type sigma;
        const int size;
    public:
        Gaussian(const result_type& mu_=result_type::Zero(), const matrix_type& sigma_=matrix_type::Identity(), const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : base_type(Eigen::hjoin(mu_.real(), mu_.imag()), 0.5*Eigen::quadjoin(sigma_.real(), sigma_.imag(), -sigma_.imag(), sigma_.real()), seed, 2*size_), mu(mu_), sigma(sigma_), size(size_)
        {
        }

        result_type operator()()
        {
            real_type tmp = base_type::operator()();
            result_type retval(size);
            for (int i = 0; i < size; i++) {
                retval(i).real(tmp(i));
                retval(i).imag(tmp(size+i));
            }
            return retval;
        }

        result_type mean() const
        {
            return mu;
        }

        result_type variance() const
        {
            return sigma;
        }
    };

    /*! @class
        @brief Wishart分布に従う確率変数の関数オブジェクト
        @tparam T double
    */
    template <typename T, class Ignored = void>
    class Wishart;

    /*! @brief 実ウィシャート分布
    */
    template <typename T, int n>
    class Wishart <Eigen::Matrix <T, n, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using type = Wishart<Eigen::Matrix<T, n, n>>;
        using result_type = Eigen::Matrix <T, n, n>;
        using v_type = Eigen::Vector<T, n>;
        using N_type = Gaussian<v_type>;
    private:
        const int size;
        const result_type sigma;
        const int k;
        N_type gaussian;
    public:
        Wishart(const result_type& sigma_=result_type::Identity(), const int k_=1, const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : size(size_), sigma(sigma_), k(k_), gaussian(v_type::Zero(size_), sigma_, seed, size_)
        {
        }

        result_type operator()()
        {
            result_type retval = result_type::Zero(size, size);
            for (int i = 0; i < k; i++) {
                const v_type x = gaussian();
                retval += x * x.transpose();
            }
            return retval;
        }

        result_type mean() const
        {
            return k * sigma;
        }
    };

    /*! @brief 複素ウィシャート分布
    */
    template <typename T, int n>
    class Wishart <Eigen::Matrix <std::complex<T>, n, n>, typename std::enable_if <std::is_floating_point <T>::value>::type> {
    public:
        using type = Wishart<Eigen::Matrix<std::complex<T>, n, n>>;
        using result_type = Eigen::Matrix <std::complex<T>, n, n>;
        using v_type = Eigen::Vector<std::complex<T>, n>;
        using N_type = Gaussian<v_type>;
    private:
        const int size;
        const result_type sigma;
        const int k;
        N_type gaussian;
    public:
        Wishart(const result_type& sigma_=result_type::Identity(), const int k_=1, const std::random_device::result_type seed=std::random_device()(), const int size_=n)
            : size(size_), sigma(sigma_), k(k_), gaussian(v_type::Zero(size_), sigma_, seed, size_)
        {
        }

        result_type operator()()
        {
            result_type retval = result_type::Zero(size, size);
            for (int i = 0; i < k; i++) {
                const v_type x = gaussian();
                retval += x * x.adjoint();
            }
            return retval;
        }

        result_type mean() const
        {
            return k * sigma;
        }
    };

    /*! @class
        @brief 各要素が一様分布に従うランダム行列確率変数の関数オブジェクト
        @tparam T double
    */
    template <typename T>
    class ElemUniform;

    /*! @brief 各要素が一様分布に従うランダム行列分布
    */
    template <typename T, int m, int n>
    class ElemUniform <Eigen::Matrix <T, m, n>> {
    public:
        using type = ElemUniform<Eigen::Matrix<T, m, n>>;
        using result_type = Eigen::Matrix <T, m, n>;
        using param_type = T;
        using ElemDistribution_type = Uniform<T>;
    private:
        const int rows;
        const int cols;
        const param_type a;
        const param_type b;
        ElemDistribution_type uniform;
    public:
        ElemUniform(param_type a_=ElemDistribution_type::default_min(), param_type b_=ElemDistribution_type::default_max(), std::random_device::result_type seed=std::random_device()(), const int rows_=m, const int cols_=n)
            : uniform(a_, b_, seed), rows(rows_), cols(cols_), a(a_), b(b_)
        {
        }

        result_type operator()()
        {
            result_type retval;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    retval(i, j) = uniform();
                }
            }
            return retval;
        }

        param_type elemmin() const
        {
            return a;
        }

        param_type elemmax() const
        {
            return b;
        }
    };
}

namespace Eigen {
    /*! @brief 各要素が一様分布に従う確率変数によって決定される行列を返す関数
        @tparam rv_type 確率変数の型
        @tparam m 行サイズ
        @tparam n 列サイズ
        @tparam elem_type 要素の型．デフォルトはelem_typeと同じ
        @tparam mx m=Dynamicのときの行サイズ
        @tparam nx n=Dynamicのときの列サイズ
    */
    template <typename rv_type, int m, int n, typename elem_type = rv_type, int mx = Eigen::Dynamic, int nx = Eigen::Dynamic>
    Matrix <elem_type, m, n> rand_uniform(const rv_type &a, const rv_type &b, const std::random_device::result_type seed=std::random_device()())
    {
        const int                rows = (m == Eigen::Dynamic) ? mx : m;
        const int                cols = (n == Eigen::Dynamic) ? nx : n;
        Matrix <elem_type, m, n> retval(rows, cols);
        std::Uniform <rv_type>  uniform(a, b, seed);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = uniform();
            }
        }

        return retval;
    }

    /*! @brief 各要素が正規分布に従う確率変数によって決定される行列を返す関数
        @tparam rv_type 確率変数の型
        @tparam m 行サイズ
        @tparam n 列サイズ
        @tparam elem_type 要素の型．デフォルトはelem_typeと同じ
        @tparam mx m=Dynamicのときの行サイズ
        @tparam nx n=Dynamicのときの列サイズ
    */
    template <typename rv_type, int m, int n, typename elem_type = rv_type, int mx = Eigen::Dynamic, int nx = Eigen::Dynamic>
    Matrix <elem_type, m, n> rand_normal(const rv_type &mu, const rv_type &sigma, const std::random_device::result_type seed=std::random_device()())
    {
        const int                rows = (m == Eigen::Dynamic) ? mx : m;
        const int                cols = (n == Eigen::Dynamic) ? nx : n;
        Matrix <elem_type, m, n> retval(rows, cols);
        std::Gaussian <rv_type>  gaussian(mu, sigma, seed);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                retval(i, j) = gaussian();
            }
        }

        return retval;
    }
}

#endif
