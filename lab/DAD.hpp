/*! @file
    @brief diagonal matrix scaling
    @author yanteyon10
    @data 1/24
*/
#ifndef DAD_HPP
#define DAD_HPP

#include <Newton_Raphson2>
#include <randeigen>

namespace linear_algebra {
    /*! @struct
        @brief diagonal matrix scaling方程式の関数オブジェクト
        @tparam Matrix 被スケーリング行列の型
    */
    template <typename Matrix, class Is_complex = void>
    struct DAD_func;

    template <typename T, int n>
    struct DAD_func <Eigen::Matrix <T, n, n>, typename std::enable_if <!std::is_complex <T>::value>::type>
    {
    public:
        // 記法
        using scalar_type = T;
        static constexpr int cp_dim = n;
        using vector_type = Eigen::Vector <scalar_type, cp_dim>;
        using matrix_type = Eigen::Matrix <scalar_type, cp_dim, cp_dim>;
        using type        = std::function <vector_type(const vector_type&)>;
    protected:
        const int         dim;
        const matrix_type A;
        const vector_type a;
    public:
        DAD_func(const matrix_type &A_, const vector_type &a_, const int dim_=n)
            : dim(dim_), A(A_), a(a_)
        {
        }

        vector_type operator()(const vector_type &x) const
        {
            return x.asDiagonal() * A * x - a;
        }
    };

    template <typename T, int n>
    struct DAD_func <Eigen::Matrix <T, n, n>, typename std::enable_if <std::is_complex <T>::value>::type>
    {
    public:
        // 記法
        using scalar_type = T;
        static constexpr int cp_dim = n;
        using vector_type = Eigen::Vector <scalar_type, cp_dim>;
        using matrix_type = Eigen::Matrix <scalar_type, cp_dim, cp_dim>;

        using scalarR_type = typename std::decomplexify <scalar_type>::type;
        static constexpr int cp_dim2 = Eigen::Size::multi(cp_dim, 2);
        using vectorR_type  = Eigen::Vector <scalarR_type, cp_dim>;
        using vector2R_type = Eigen::Vector <scalarR_type, cp_dim2>;
        using matrix2R_type = Eigen::Matrix <scalarR_type, cp_dim2, cp_dim2>;

        using type = std::function <vector2R_type(const vector2R_type&)>;
    protected:
        const int         dim;
        const int         dim2;
        const matrix_type A;
        const vector_type a;
    public:
        DAD_func(const matrix_type &A_, const vectorR_type &a_, const int dim_=n)
            : dim(dim_), dim2(dim_ * 2), A(A_), a(a_.template cast <scalar_type>())
        {
        }

        vector2R_type operator()(const vector2R_type &z) const
        {
            vector_type x;
            x.real() = z.template block <cp_dim, 1>(0, 0, dim, 1);
            x.imag() = z.template block <cp_dim, 1>(dim, 0, dim, 1);
            vector_type   f = x.conjugate().asDiagonal() * A * x - a;
            vector2R_type retval;
            retval.template block <cp_dim, 1>(0, 0, dim, 1)   = f.real();
            retval.template block <cp_dim, 1>(dim, 0, dim, 1) = f.imag();

            return retval;
        }
    };

    /*! @struct
        @brief diagonal matrix scaling方程式のヤコビ行列の関数オブジェクト
        @tparam Matrix 被スケーリング行列の型
    */
    template <typename Matrix, class Is_complex = void>
    struct DAD_Jacobi;

    template <typename T, int n>
    struct DAD_Jacobi <Eigen::Matrix <T, n, n>, typename std::enable_if <!std::is_complex <T>::value>::type>
    {
    public:
        // 記法
        using scalar_type = T;
        static constexpr int cp_dim = n;
        using vector_type = Eigen::Vector <scalar_type, cp_dim>;
        using matrix_type = Eigen::Matrix <scalar_type, cp_dim, cp_dim>;
        using type        = std::function <vector_type(const vector_type&)>;
    protected:
        const int         dim;
        const matrix_type A;
    public:
        DAD_Jacobi(const matrix_type &A_, const int dim_=n)
            : dim(dim_), A(A_)
        {
        }

        matrix_type operator()(const vector_type &x) const
        {
            matrix_type tmp = x.asDiagonal() * A;
            tmp += (A * x).eval().asDiagonal();

            return tmp;
        }
    };

    template <typename T, int n>
    struct DAD_Jacobi <Eigen::Matrix <T, n, n>, typename std::enable_if <std::is_complex <T>::value>::type>
    {
    public:
        // 記法
        using scalar_type = T;
        static constexpr int cp_dim = n;
        using vector_type = Eigen::Vector <scalar_type, cp_dim>;
        using matrix_type = Eigen::Matrix <scalar_type, cp_dim, cp_dim>;

        using scalarR_type = typename std::decomplexify <scalar_type>::type;
        static constexpr int cp_dim2 = Eigen::Size::multi(cp_dim, 2);
        using vectorR_type  = Eigen::Vector <scalarR_type, cp_dim>;
        using vector2R_type = Eigen::Vector <scalarR_type, cp_dim2>;
        using matrix2R_type = Eigen::Matrix <scalarR_type, cp_dim2, cp_dim2>;

        using type = std::function <vector2R_type(const vector2R_type&)>;
    protected:
        const int         dim;
        const int         dim2;
        const matrix_type A;
    public:
        DAD_Jacobi(const matrix_type &A_, const int dim_=n)
            : dim(dim_), dim2(dim_ * 2), A(A_)
        {
        }

        matrix2R_type operator()(const vector2R_type &z) const
        {
            vector_type x;
            x.real() = z.template block <cp_dim, 1>(0, 0, dim, 1);
            x.imag() = z.template block <cp_dim, 1>(dim, 0, dim, 1);
            matrix_type   XA     = x.conjugate().asDiagonal() * A;
            matrix_type   diagAx = (A * x).eval().asDiagonal();
            matrix2R_type retval;
            retval.template block <cp_dim, cp_dim>(0, 0, dim, dim)     = XA.real() + diagAx.real();
            retval.template block <cp_dim, cp_dim>(dim, 0, dim, dim)   = XA.imag() + diagAx.imag();
            retval.template block <cp_dim, cp_dim>(0, dim, dim, dim)   = -XA.imag() + diagAx.imag();
            retval.template block <cp_dim, cp_dim>(dim, dim, dim, dim) = XA.real() - diagAx.real();

            return retval;
        }
    };

    /*! @class
        @brief diagonal matrix scalingを解くクラスの本体
        @tparam Matrix 被スケーリング行列の型
    */
    template <typename Matrix, class Is_complex = void>
    class DAD;

    template <typename T, int n>
    class DAD <Eigen::Matrix <T, n, n>, typename std::enable_if <!std::is_complex <T>::value>::type>
        : public equation::Newton_Raphson2 <std::function <Eigen::Vector <T, n>(const Eigen::Vector <T, n>&)> >
    {
    public:
        // 記法
        using scalar_type = T;
        static constexpr int cp_dim = n;
        using vector_type = Eigen::Vector <scalar_type, cp_dim>;
        using matrix_type = Eigen::Matrix <scalar_type, cp_dim, cp_dim>;
        using func_type   = std::function <vector_type(const vector_type&)>;
        using Jacobi_type = std::function <matrix_type(const vector_type&)>;
        using type        = DAD <matrix_type>;
        using base_type   = equation::Newton_Raphson2 <func_type>;
    protected:
        const matrix_type A;
        const vector_type a;
    public:
        static int max_trial;
    public:
        /*! @brief コンストラクタ
        */
        DAD(const matrix_type &A_, const vector_type &a_=vector_type::Ones())
            : A(A_), a(a_), base_type(func_type(DAD_func <matrix_type>(A_, a_)), Jacobi_type(DAD_Jacobi <matrix_type>(A_)))
        {
        }

        /*! @brief 解が見つかるまで初期値を変えながら解く
        */
        vector_type solve_again(std::ostream &os=std::cout, const vector_type &x_0_=vector_type::Ones())
        {
            std::Gaussian <vector_type> gaussian;
            vector_type x_0 = x_0_;
            for (int i = 0; i < max_trial; i++) {
                // 解く
                const vector_type x = solve(os, x_0);

                // チェック
                if (base_type::success) {
                    return x;
                }

                // 更新
                x_0 = gaussian();
            }
            assert(false);
        }
    };

    template <typename T, int n>
    int DAD <Eigen::Matrix <T, n, n>, typename std::enable_if <!std::is_complex <T>::value>::type>::max_trial = 10;

    template <typename T, int n>
    class DAD <Eigen::Matrix <T, n, n>, typename std::enable_if <std::is_complex <T>::value>::type>
        : public equation::Newton_Raphson2 <std::function <Eigen::Vector <typename std::decomplexify <T>::type, Eigen::Size::multi(n, 2)>(const Eigen::Vector <typename std::decomplexify <T>::type, Eigen::Size::multi(n, 2)>&)> >
    {
    public:
        // 記法
        using scalar_type = T;
        static constexpr int cp_dim = n;
        using vector_type = Eigen::Vector <scalar_type, cp_dim>;
        using matrix_type = Eigen::Matrix <scalar_type, cp_dim, cp_dim>;

        using scalarR_type = typename std::decomplexify <scalar_type>::type;
        static constexpr int cp_dim2 = Eigen::Size::multi(cp_dim, 2);
        using vectorR_type  = Eigen::Vector <scalarR_type, cp_dim>;
        using vector2R_type = Eigen::Vector <scalarR_type, cp_dim2>;
        using matrix2R_type = Eigen::Matrix <scalarR_type, cp_dim2, cp_dim2>;

        using func_type   = std::function <vector2R_type(const vector2R_type&)>;
        using Jacobi_type = std::function <matrix2R_type(const vector2R_type&)>;
        using type        = DAD <matrix_type>;
        using base_type   = equation::Newton_Raphson2 <func_type>;
    protected:
        const int          dim;
        const matrix_type  A;
        const vectorR_type a;
    public:
        static int max_trial;
    public:
        /*! @brief コンストラクタ
        */
        DAD(const matrix_type &A_, const vectorR_type &a_=vectorR_type::Ones())
            : dim(A_.rows()), A(A_), a(a_), base_type(func_type(DAD_func <matrix_type>(A_, a_)), Jacobi_type(DAD_Jacobi <matrix_type>(A_)))
        {
        }

        /*! @brief 解く
        */
        vector_type solve(std::ostream &os=std::cout, const vector_type &x_0=vector_type::Ones())
        {
            vector2R_type z_0;
            z_0.template block <cp_dim, 1>(0, 0, dim, 1)   = x_0.real();
            z_0.template block <cp_dim, 1>(dim, 0, dim, 1) = x_0.imag();
            vector2R_type z = base_type::solve(os, z_0);
            vector_type   retval;
            retval.real() = z.template block <cp_dim, 1>(0, 0, dim, 1);
            retval.imag() = z.template block <cp_dim, 1>(dim, 0, dim, 1);

            return retval;
        }

        /*! @brief 解が見つかるまで初期値を変えながら解く
        */
        vector_type solve_again(std::ostream &os=std::cout, const vector_type &x_0_=vector_type::Ones())
        {
            std::Gaussian <vector_type> gaussian;
            vector_type x_0 = x_0_;
            for (int i = 0; i < max_trial; i++) {
                // 解く
                const vector_type x = solve(os, x_0);

                // チェック
                if (base_type::success) {
                    return x;
                }

                // 更新
                x_0 = gaussian();
            }
            assert(false);
        }
    };

    template <typename T, int n>
    int DAD <Eigen::Matrix <T, n, n>, typename std::enable_if <std::is_complex <T>::value>::type>::max_trial = 10;
}

#endif
