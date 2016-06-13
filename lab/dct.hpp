/*! @file
    @brief DCT（離散コサイン変換）．DCT-1とDCT-2がある
    @author templateaholic10
    @date 12/29
*/
#ifndef DCT_HPP
#define DCT_HPP

#include <fft>
#include <elemwise>

namespace Eigen {
    /*! @brief DCT-1用の偶拡張メタ関数．abcdeからabcdedcbを作る
    */
    template <typename T, int Option = Eigen::ColMajor>
    struct Evenize1;

    /*! @brief 縦ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor> {
        using type = typename Eigen::meta::expand <typename Eigen::meta::scaleup <Matrix <Scalar, Rows, Cols>, 2, 1>::type, -2, 0>::type;
    };

    /*! @brief 横ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::RowMajor> {
        using type = typename Eigen::meta::expand <typename Eigen::meta::scaleup <Matrix <Scalar, Rows, Cols>, 1, 2>::type, 0, -2>::type;
    };

    /*! @brief DCT-1用の偶拡張関数．abcdeからabcdedcbを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::ColMajor>::type * = nullptr>
    typename Evenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type evenize1(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Evenize1 <param_type, Option>::type;
        result_type retval;
        retval.template block <Rows, Cols>(0, 0, rows, cols)                             = M;
        retval.template block <Eigen::Size::sub(Rows, 2), Cols>(rows, 0, rows - 2, cols) = M.colwise().reverse().template block <Eigen::Size::sub(Rows, 2), Cols>(1, 0, rows - 2, cols);

        return retval;
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::RowMajor>::type * = nullptr>
    typename Evenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type evenize1(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Evenize1 <param_type, Option>::type;
        result_type retval;
        retval.template block <Rows, Cols>(0, 0, rows, cols)                             = M;
        retval.template block <Rows, Eigen::Size::sub(Cols, 2)>(0, cols, rows, cols - 2) = M.rowwise().reverse().template block <Rows, Eigen::Size::sub(Cols, 2)>(0, 1, rows, cols - 2);

        return retval;
    }

    template <typename Scalar, int Rows, int Cols>
    typename Evenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor>::type evenize1(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        return evenize1 <Eigen::ColMajor>(M);
    }

    template <typename Scalar, size_t N>
    typename std::array <Scalar, 2 *N - 2> evenize1(const std::array <Scalar, N> &x)
    {
        using param_type  = std::array <Scalar, N>;
        using result_type = std::array <Scalar, 2 *N - 2>;
        result_type retval;
        std::copy(x.begin(), x.end(), retval.begin());
        std::reverse_copy(x.begin() + 1, x.end() - 1, retval.begin() + N);

        return retval;
    }

    template <typename Scalar>
    typename std::vector <Scalar> evenize1(const std::vector <Scalar> &x)
    {
        using param_type  = std::vector <Scalar>;
        using result_type = std::vector <Scalar>;
        const int   N = x.size();
        result_type retval(2 * N - 2);
        std::copy(x.begin(), x.end(), retval.begin());
        std::reverse_copy(x.begin() + 1, x.end() - 1, retval.begin() + N);

        return retval;
    }

    /*! @brief DCT-1用の偶拡張から情報を取り出すメタ関数．abcdedcbからabcdeを作る
    */
    template <typename T, int Option = Eigen::ColMajor>
    struct Deevenize1;

    /*! @brief 縦ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Deevenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor> {
        using type = typename Eigen::meta::scaledown <typename Eigen::meta::expand <Matrix <Scalar, Rows, Cols>, 2, 0>::type, 2, 1>::type;
    };

    /*! @brief 横ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Deevenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::RowMajor> {
        using type = typename Eigen::meta::scaledown <typename Eigen::meta::expand <Matrix <Scalar, Rows, Cols>, 0, 2>::type, 1, 2>::type;
    };

    /*! @brief DCT-1用の偶拡張から情報を取り出す関数．abcdedcbからabcdeを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::ColMajor>::type * = nullptr>
    typename Deevenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type deevenize1(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Deevenize1 <param_type, Option>::type;

        return M.template block <result_type::RowsAtCompileTime, result_type::ColsAtCompileTime>(0, 0, (rows + 2) / 2, cols).eval();
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::RowMajor>::type * = nullptr>
    typename Deevenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type deevenize1(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Deevenize1 <param_type, Option>::type;

        return M.template block <result_type::RowsAtCompileTime, result_type::ColsAtCompileTime>(0, 0, rows, (cols + 2) / 2).eval();
    }

    template <typename Scalar, int Rows, int Cols>
    typename Deevenize1 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor>::type deevenize1(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        return deevenize1 <Eigen::ColMajor>(M);
    }

    template <typename Scalar, size_t Neven>
    typename std::array <Scalar, (Neven + 2) / 2> deevenize1(const std::array <Scalar, Neven> &x)
    {
        constexpr int N = (Neven + 2) / 2;
        using param_type  = std::array <Scalar, Neven>;
        using result_type = std::array <Scalar, N>;
        result_type retval;
        std::copy(x.begin(), x.begin() + N, retval.begin());

        return retval;
    }

    template <typename Scalar>
    typename std::vector <Scalar> deevenize1(const std::vector <Scalar> &x)
    {
        const int Neven = x.size();
        const int N     = (Neven + 2) / 2;
        using param_type  = std::vector <Scalar>;
        using result_type = std::vector <Scalar>;
        result_type retval(N);
        std::copy(x.begin(), x.begin() + N, retval.begin());

        return retval;
    }

    /*! @brief 1変量時系列のDCT-1
        @tparam To 実数型または複素数型．指定しないと実数型になる
        @tparam From 実数型または複素数型
        @tparam N 系列の長さ
        @param x 1変量時系列
    */

    /*! @brief std::array用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From, size_t N>
    std::array <To, N> dct1(const std::array <From, N> &x)
    {
        return (std::Elemwise <std::array <To, N> >(deevenize1(fft <To>(evenize1(x)))) * 0.5).array();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <size_t N, typename From>
    std::array <typename std::decomplexify <From>::type, N> dct1(const std::array <From, N> &x)
    {
        return dct1 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief std::vector用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From>
    std::vector <To> dct1(const std::vector <From> &x)
    {
        return (std::Elemwise <std::vector <To> >(deevenize1(fft <To>(evenize1(x)))) * 0.5).vector();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <typename From>
    std::vector <typename std::decomplexify <From>::type> dct1(const std::vector <From> &x)
    {
        return dct1 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief 多変量時系列のDCT-1
        @tparam To 実数型または複素数型
        指定しないと複素数型になる
        @tparam From 実数型または複素数型
        @tparam Rows 行数
        @tparam Cols 列数
        @param X データ行列
        @param option 時間方向の縦横．デフォルト値はEigen::ColMajorでこのとき時間方向は縦
    */

    /*! @brief Eigen::Matrix用
    */

    /*! @brief 型指定する，方向指定する場合
    */
    template <typename To, int Option, typename From, int Rows, int Cols>
    Eigen::Matrix <To, Rows, Cols> dct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return deevenize1 <Option>(fft <To>(evenize1 <Option>(X), Option)) * 0.5;
    }

    /*! @brief 型指定する，方向指定しない場合を担保
    */
    template <typename To, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> dct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return dct1 <To, Eigen::ColMajor>(X);
    }

    /*! @brief 型指定しない，方向指定する場合を担保
    */
    template <int Option, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> dct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return dct1 <typename std::decomplexify <From>::type, Option>(X);
    }

    /*! @brief 型指定しない，方向指定しない場合を担保
    */
    template <int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> dct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return dct1 <typename std::decomplexify <From>::type, Eigen::ColMajor>(X);
    }

    /*! @brief 1変量時系列のiDCT-1
        @tparam To 実数型または複素数型．指定しないと実数型になる
        @tparam From 実数型または複素数型
        @tparam N 系列の長さ
        @param x 1変量時系列
    */

    /*! @brief std::array用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From, size_t N>
    std::array <To, N> idct1(const std::array <From, N> &x)
    {
        return (std::Elemwise <std::array <To, N> >(deevenize1(ifft <To>(evenize1(x)))) * 2.).array();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <size_t N, typename From>
    std::array <typename std::decomplexify <From>::type, N> idct1(const std::array <From, N> &x)
    {
        return idct1 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief std::vector用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From>
    std::vector <To> idct1(const std::vector <From> &x)
    {
        return (std::Elemwise <std::vector <To> >(deevenize1(ifft <To>(evenize1(x)))) * 2.).vector();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <typename From>
    std::vector <typename std::decomplexify <From>::type> idct1(const std::vector <From> &x)
    {
        return idct1 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief 多変量時系列のDCT-1
        @tparam To 実数型または複素数型
        指定しないと複素数型になる
        @tparam From 実数型または複素数型
        @tparam Rows 行数
        @tparam Cols 列数
        @param X データ行列
        @param option 時間方向の縦横．デフォルト値はEigen::ColMajorでこのとき時間方向は縦
    */

    /*! @brief Eigen::Matrix用
    */

    /*! @brief 型指定する，方向指定する場合
    */
    template <typename To, int Option, typename From, int Rows, int Cols>
    Eigen::Matrix <To, Rows, Cols> idct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return deevenize1 <Option>(ifft <To>(evenize1 <Option>(X), Option)) * 2.;
    }

    /*! @brief 型指定する，方向指定しない場合を担保
    */
    template <typename To, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> idct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return idct1 <To, Eigen::ColMajor>(X);
    }

    /*! @brief 型指定しない，方向指定する場合を担保
    */
    template <int Option, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> idct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return idct1 <typename std::decomplexify <From>::type, Option>(X);
    }

    /*! @brief 型指定しない，方向指定しない場合を担保
    */
    template <int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> idct1(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return idct1 <typename std::decomplexify <From>::type, Eigen::ColMajor>(X);
    }

    /*! @brief DCT-2用の偶拡張メタ関数．abcdeからabcdedcbを作る
    */
    template <typename T, int Option = Eigen::ColMajor>
    struct Evenize2;

    /*! @brief 縦ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor> {
        using type = typename Eigen::meta::scaleup <Matrix <Scalar, Rows, Cols>, 4, 1>::type;
    };

    /*! @brief 横ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::RowMajor> {
        using type = typename Eigen::meta::scaleup <Matrix <Scalar, Rows, Cols>, 1, 4>::type;
    };

    /*! @brief DCT-2用の偶拡張関数．abcdeから0a0b0c0d0e0e0d0c0b0aを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::ColMajor>::type * = nullptr>
    typename Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type evenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Evenize2 <param_type, Option>::type;
        result_type retval;
        for (int i = 0; i < rows; i++) {
            retval.row(2 * i).fill(0);
            retval.row(2 * i + 1) = M.row(i);
        }
        for (int i = rows; i < 2 * rows; i++) {
            retval.row(2 * i).fill(0);
            retval.row(2 * i + 1) = M.row(2 * rows - 1 - i);
        }

        return retval;
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::RowMajor>::type * = nullptr>
    typename Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type evenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Evenize2 <param_type, Option>::type;
        result_type retval;
        for (int j = 0; j < cols; j++) {
            retval.col(2 * j).fill(0);
            retval.col(2 * j + 1) = M.col(j);
        }
        for (int j = cols; j < 2 * cols; j++) {
            retval.col(2 * j).fill(0);
            retval.col(2 * j + 1) = M.col(2 * cols - 1 - j);
        }

        return retval;
    }

    template <typename Scalar, int Rows, int Cols>
    typename Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor>::type evenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        return evenize2 <Eigen::ColMajor>(M);
    }

    template <typename Scalar, size_t N>
    typename std::array <Scalar, 4 *N> evenize2(const std::array <Scalar, N> &x)
    {
        using param_type  = std::array <Scalar, N>;
        using result_type = std::array <Scalar, 4 *N>;
        result_type retval;
        for (size_t i = 0; i < N; i++) {
            retval[2 * i] = 0;
            retval[2 * i + 1] = x[i];
        }
        for (size_t i = N; i < 2 * N; i++) {
            retval[2 * i] = 0;
            retval[2 * i + 1] = x[2 * N - 1 - i];
        }

        return retval;
    }

    template <typename Scalar>
    typename std::vector <Scalar> evenize2(const std::vector <Scalar> &x)
    {
        using param_type  = std::vector <Scalar>;
        using result_type = std::vector <Scalar>;
        const int   N = x.size();
        result_type retval(4 * N);
        for (size_t i = 0; i < N; i++) {
            retval[2 * i] = 0;
            retval[2 * i + 1] = x[i];
        }
        for (size_t i = N; i < 2 * N; i++) {
            retval[2 * i] = 0;
            retval[2 * i + 1] = x[2 * N - 1 - i];
        }

        return retval;
    }

    /*! @brief DCT-2用の偶拡張から情報を取り出すメタ関数．abcdedcbからabcdeを作る
    */
    template <typename T, int Option = Eigen::ColMajor>
    struct Deevenize2;

    /*! @brief 縦ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor> {
        using type = typename Eigen::meta::scaledown <Matrix <Scalar, Rows, Cols>, 4, 1>::type;
    };

    /*! @brief 横ベクトル
    */
    template <typename Scalar, int Rows, int Cols>
    struct Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::RowMajor> {
        using type = typename Eigen::meta::scaledown <Matrix <Scalar, Rows, Cols>, 1, 4>::type;
    };

    /*! @brief DCT-2用の偶拡張から情報を取り出す関数．abcdedcbからabcdeを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::ColMajor>::type * = nullptr>
    typename Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type deevenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Deevenize2 <param_type, Option>::type;

        return M.template block <result_type::RowsAtCompileTime, result_type::ColsAtCompileTime>(0, 0, rows / 4, cols).eval();
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::RowMajor>::type * = nullptr>
    typename Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type deevenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Deevenize2 <param_type, Option>::type;

        return M.template block <result_type::RowsAtCompileTime, result_type::ColsAtCompileTime>(0, 0, rows, cols / 4).eval();
    }

    template <typename Scalar, int Rows, int Cols>
    typename Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor>::type deevenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        return deevenize2 <Eigen::ColMajor>(M);
    }

    template <typename Scalar, size_t Neven>
    typename std::array <Scalar, Neven / 4> deevenize2(const std::array <Scalar, Neven> &x)
    {
        constexpr int N = Neven / 4;
        using param_type  = std::array <Scalar, Neven>;
        using result_type = std::array <Scalar, N>;
        result_type retval;
        std::copy(x.begin(), x.begin() + N, retval.begin());

        return retval;
    }

    template <typename Scalar>
    typename std::vector <Scalar> deevenize2(const std::vector <Scalar> &x)
    {
        const int Neven = x.size();
        const int N     = Neven / 4;
        using param_type  = std::vector <Scalar>;
        using result_type = std::vector <Scalar>;
        result_type retval(N);
        std::copy(x.begin(), x.begin() + N, retval.begin());

        return retval;
    }

    /*! @brief IDCT-2用の偶拡張関数．abcdeからabcde-e-d-c-b-a-a-b-c-d-eedcbaを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::ColMajor>::type * = nullptr>
    typename Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type ievenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Evenize2 <param_type, Option>::type;
        result_type retval;
        retval.template block<Rows, Cols>(0, 0, rows, cols) = M;
        retval.row(rows).fill(Scalar(0.));
        retval.template block<Eigen::Size::sub(Rows, 1), Cols>(rows+1, 0, rows-1, cols) = - M.template block<Eigen::Size::sub(Rows, 1), Cols>(1, 0, rows-1, cols).colwise().reverse();
        retval.template block<Rows, Cols>(2*rows, 0, rows, cols) = - M;
        retval.row(3*rows).fill(Scalar(0.));
        retval.template block<Eigen::Size::sub(Rows, 1), Cols>(3*rows+1, 0, rows-1, cols) = M.template block<Eigen::Size::sub(Rows, 1), Cols>(1, 0, rows-1, cols).colwise().reverse();

        return retval;
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::RowMajor>::type * = nullptr>
    typename Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type ievenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Evenize2 <param_type, Option>::type;
        result_type retval;
        retval.template block<Rows, Cols>(0, 0, rows, cols) = M;
        retval.col(cols).fill(Scalar(0.));
        retval.template block<Rows, Eigen::Size::sub(Cols, 1)>(0, cols+1, rows, cols-1) = - M.template block<Rows, Eigen::Size::sub(Cols, 1)>(0, 1, rows, cols-1).rowwise().reverse();
        retval.template block<Rows, Cols>(0, 2*cols, rows, cols) = - M;
        retval.col(3*cols).fill(Scalar(0.));
        retval.template block<Rows, Eigen::Size::sub(Cols, 1)>(0, 3*cols+1, rows, cols-1) = M.template block<Rows, Eigen::Size::sub(Cols, 1)>(0, 1, rows, cols-1).rowwise().reverse();

        return retval;
    }

    template <typename Scalar, int Rows, int Cols>
    typename Evenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor>::type ievenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        return evenize2 <Eigen::ColMajor>(M);
    }

    template <typename Scalar, size_t N>
    typename std::array <Scalar, 4 *N> ievenize2(const std::array <Scalar, N> &x)
    {
        using param_type  = std::array <Scalar, N>;
        using result_type = std::array <Scalar, 4 *N>;
        result_type retval;
        std::copy(x.cbegin(), x.cend(), retval.begin());
        retval[N] = static_cast<Scalar>(0.);
        std::transform(x.crbegin(), x.crend()-1, retval.begin()+(N+1), [](const Scalar from){return -from;});
        std::transform(x.cbegin(), x.cend(), retval.begin()+2*N, [](const Scalar from){return -from;});
        retval[3*N] = static_cast<Scalar>(0.);
        std::reverse_copy(x.cbegin()+1, x.cend(), retval.begin()+(3*N+1));

        return retval;
    }

    template <typename Scalar>
    typename std::vector <Scalar> ievenize2(const std::vector <Scalar> &x)
    {
        using param_type  = std::vector <Scalar>;
        using result_type = std::vector <Scalar>;
        const int   N = x.size();
        result_type retval(4 * N);
        std::copy(x.cbegin(), x.cend(), retval.begin());
        retval[N] = static_cast<Scalar>(0.);
        std::transform(x.crbegin(), x.crend()-1, retval.begin()+(N+1), [](const Scalar from){return -from;});
        std::transform(x.cbegin(), x.cend(), retval.begin()+2*N, [](const Scalar from){return -from;});
        retval[3*N] = static_cast<Scalar>(0.);
        std::reverse_copy(x.cbegin()+1, x.cend(), retval.begin()+(3*N+1));

        return retval;
    }

    /*! @brief IDCT-2用の偶拡張から情報を取り出す関数．0a0b0c0d0e0e0d0c0b0aからabcdeを作る
    */
    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::ColMajor>::type * = nullptr>
    typename Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type ideevenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Deevenize2 <param_type, Option>::type;
        result_type retval;
        for (int i = 0; i < retval.rows(); i++) {
            retval.row(i) = M.row(2*i+1);
        }

        return retval;
    }

    template <int Option, typename Scalar, int Rows, int Cols, typename std::enable_if <Option == Eigen::RowMajor>::type * = nullptr>
    typename Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Option>::type ideevenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        const int rows = M.rows();
        const int cols = M.cols();
        using param_type  = Eigen::Matrix <Scalar, Rows, Cols>;
        using result_type = typename Deevenize2 <param_type, Option>::type;
        result_type retval;
        for (int j = 0; j < retval.cols(); j++) {
            retval.col(j) = M.col(2*j+1);
        }

        return retval;
    }

    template <typename Scalar, int Rows, int Cols>
    typename Deevenize2 <Eigen::Matrix <Scalar, Rows, Cols>, Eigen::ColMajor>::type ideevenize2(const Eigen::Matrix <Scalar, Rows, Cols> &M)
    {
        return deevenize2 <Eigen::ColMajor>(M);
    }

    template <typename Scalar, size_t Neven>
    typename std::array <Scalar, Neven / 4> ideevenize2(const std::array <Scalar, Neven> &x)
    {
        constexpr int N = Neven / 4;
        using param_type  = std::array <Scalar, Neven>;
        using result_type = std::array <Scalar, N>;
        result_type retval;
        for (size_t i = 0; i < N; i++) {
            retval[i] = x[2*i+1];
        }

        return retval;
    }

    template <typename Scalar>
    typename std::vector <Scalar> ideevenize2(const std::vector <Scalar> &x)
    {
        const int Neven = x.size();
        const int N     = Neven / 4;
        using param_type  = std::vector <Scalar>;
        using result_type = std::vector <Scalar>;
        result_type retval(N);
        for (size_t i = 0; i < N; i++) {
            retval[i] = x[2*i+1];
        }

        return retval;
    }

    /*! @brief 1変量時系列のDCT-2
        @tparam To 実数型または複素数型．指定しないと実数型になる
        @tparam From 実数型または複素数型
        @tparam N 系列の長さ
        @param x 1変量時系列
    */

    /*! @brief std::array用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From, size_t N>
    std::array <To, N> dct2(const std::array <From, N> &x)
    {
        _PRINTSEQ(evenize2(x))
        _PRINTSEQ(fft <To>(evenize2(x)))
        _PRINTSEQ(deevenize2(fft <To>(evenize2(x))))
        return (std::Elemwise <std::array <To, N> >(deevenize2(fft <To>(evenize2(x)))) * 0.5).array();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <size_t N, typename From>
    std::array <typename std::decomplexify <From>::type, N> dct2(const std::array <From, N> &x)
    {
        return dct2 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief std::vector用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From>
    std::vector <To> dct2(const std::vector <From> &x)
    {
        return (std::Elemwise <std::vector <To> >(deevenize2(fft <To>(evenize2(x)))) * 0.5).vector();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <typename From>
    std::vector <typename std::decomplexify <From>::type> dct2(const std::vector <From> &x)
    {
        return dct2 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief 多変量時系列のDCT-2
        @tparam To 実数型または複素数型
        指定しないと複素数型になる
        @tparam From 実数型または複素数型
        @tparam Rows 行数
        @tparam Cols 列数
        @param X データ行列
        @param option 時間方向の縦横．デフォルト値はEigen::ColMajorでこのとき時間方向は縦
    */

    /*! @brief Eigen::Matrix用
    */

    /*! @brief 型指定する，方向指定する場合
    */
    template <typename To, int Option, typename From, int Rows, int Cols>
    Eigen::Matrix <To, Rows, Cols> dct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        _PRINT(evenize2<Option>(X))
        _PRINT(fft <To>(evenize2<Option>(X), Option))
        _PRINT(deevenize2<Option>(fft <To>(evenize2<Option>(X), Option)))
        return deevenize2 <Option>(fft <To>(evenize2 <Option>(X), Option)) * 0.5;
    }

    /*! @brief 型指定する，方向指定しない場合を担保
    */
    template <typename To, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> dct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return dct2 <To, Eigen::ColMajor>(X);
    }

    /*! @brief 型指定しない，方向指定する場合を担保
    */
    template <int Option, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> dct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return dct2 <typename std::decomplexify <From>::type, Option>(X);
    }

    /*! @brief 型指定しない，方向指定しない場合を担保
    */
    template <int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> dct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return dct2 <typename std::decomplexify <From>::type, Eigen::ColMajor>(X);
    }

    /*! @brief 1変量時系列のiDCT-2
        @tparam To 実数型または複素数型．指定しないと実数型になる
        @tparam From 実数型または複素数型
        @tparam N 系列の長さ
        @param x 1変量時系列
    */

    /*! @brief std::array用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From, size_t N>
    std::array <To, N> idct2(const std::array <From, N> &x)
    {
        _PRINTSEQ(ievenize2(x))
        _PRINTSEQ(ifft <To>(ievenize2(x)))
        _PRINTSEQ(ideevenize2(ifft <To>(ievenize2(x))))
        return (std::Elemwise <std::array <To, N> >(ideevenize2(ifft <To>(ievenize2(x)))) * 2.).array();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <size_t N, typename From>
    std::array <typename std::decomplexify <From>::type, N> idct2(const std::array <From, N> &x)
    {
        return idct2 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief std::vector用
    */

    /*! @brief 型指定する場合
    */
    template <typename To, typename From>
    std::vector <To> idct2(const std::vector <From> &x)
    {
        return (std::Elemwise <std::vector <To> >(deevenize2(ifft <To>(evenize2(x)))) * 2.).vector();
    }

    /*! @brief 型指定しない場合は実数型になる．原系列が実数型なら変換後も実数型になるため
    */
    template <typename From>
    std::vector <typename std::decomplexify <From>::type> idct2(const std::vector <From> &x)
    {
        return idct2 <typename std::decomplexify <From>::type>(x);
    }

    /*! @brief 多変量時系列のDCT-2
        @tparam To 実数型または複素数型
        指定しないと複素数型になる
        @tparam From 実数型または複素数型
        @tparam Rows 行数
        @tparam Cols 列数
        @param X データ行列
        @param option 時間方向の縦横．デフォルト値はEigen::ColMajorでこのとき時間方向は縦
    */

    /*! @brief Eigen::Matrix用
    */

    /*! @brief 型指定する，方向指定する場合
    */
    template <typename To, int Option, typename From, int Rows, int Cols>
    Eigen::Matrix <To, Rows, Cols> idct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        _PRINT(ievenize2<Option>(X))
        _PRINT(ifft <To>(ievenize2<Option>(X), Option))
        _PRINT(ideevenize2<Option>(ifft <To>(ievenize2<Option>(X), Option)))
        return ideevenize2 <Option>(ifft <To>(ievenize2 <Option>(X), Option)) * 2.;
    }

    /*! @brief 型指定する，方向指定しない場合を担保
    */
    template <typename To, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> idct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return idct2 <To, Eigen::ColMajor>(X);
    }

    /*! @brief 型指定しない，方向指定する場合を担保
    */
    template <int Option, int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> idct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return idct2 <typename std::decomplexify <From>::type, Option>(X);
    }

    /*! @brief 型指定しない，方向指定しない場合を担保
    */
    template <int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::decomplexify <From>::type, Rows, Cols> idct2(const Eigen::Matrix <From, Rows, Cols> &X)
    {
        return idct2 <typename std::decomplexify <From>::type, Eigen::ColMajor>(X);
    }
}

#endif
