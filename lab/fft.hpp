/*! @file
    @brief FFT
    @author yanteyon10
    @date 12/9
*/
#ifndef FFT_HPP
#define FFT_HPP

#include <iostream>
#include <vector>
#include <array>
#include <excomplex>
#include <exeigen>
#include <unsupported/Eigen/FFT>
#include <algorithm>
#include <functional>

namespace Eigen {
    /*! @brief 1変量時系列のFFT．
        @tparam To 実数型または複素数型
        指定しないと複素数型になる
        @tparam From 実数型または複素数型
        @tparam N 系列の長さ
        @param x 1変量時系列
    */

    /*! @brief std::array用
    */

    /*! @brief fft<std::complex<double>>の場合を担保
    */
    template <typename To, typename From, size_t N, typename std::enable_if <std::is_same <To, typename std::complexify <To>::type>::value>::type * = nullptr>
    std::array <To, N> fft(const std::array <From, N> &x)
    {
        using result_type = std::array <To, N>;
        result_type retval;
        Eigen::FFT <typename std::decomplexify <To>::type> fft;
        fft.fwd(retval.data(), x.data(), N);

        return retval;
    }

    /*! @brief 型指定しない場合を担保
    */
    template <size_t N, typename From>
    std::array <typename std::complexify<From>::type, N> fft(const std::array <From, N> &x)
    {
        return fft<typename std::complexify<From>::type>(x);
    }

    /*! @brief fft<double>の場合を担保
    */
    template <typename To, typename From, size_t N, typename std::enable_if <!std::is_same <To, typename std::complexify <To>::type>::value>::type * = nullptr>
    std::array <To, N> fft(const std::array <From, N> &x)
    {
        using tmp_type = std::array <typename std::complexify <To>::type, N>;
        tmp_type tmp(fft<typename std::complexify <To>::type>(x));
        using result_type = std::array <To, N>;
        result_type retval;
        std::transform(tmp.begin(), tmp.end(), retval.begin(), [](const typename std::complexify <To>::type &c){return std::real(c);});

        return retval;
    }

    /*! @brief std::vector用
    */
    template <typename T>
    std::vector <typename std::complexify <T>::type> fft(const std::vector <T> &x)
    {
        using result_type = std::vector <typename std::complexify <T>::type>;
        result_type                                       retval;
        Eigen::FFT <typename std::decomplexify <T>::type> fft;
        fft.fwd(retval, x);

        return retval;
    }

    /*! @brief 多変量時系列のFFT．
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

    // TODO: 時間方向横のときfftがうまくいかない

    /*! @brief fft<std::complex<double>>の場合を担保
    */
    template <typename To, typename From, int Rows, int Cols, typename std::enable_if <std::is_same <To, typename std::complexify <To>::type>::value>::type * = nullptr>
    Eigen::Matrix <To, Rows, Cols> fft(const Eigen::Matrix <From, Rows, Cols> &X, const int option = Eigen::ColMajor)
    {
        const int                                         rows = X.rows();
        const int                                         cols = X.cols();
        using result_type = Eigen::Matrix <To, Rows, Cols>;
        result_type                                       retval(rows, cols);
        Eigen::FFT <typename std::decomplexify <To>::type> fft;
        if (option == Eigen::ColMajor) {
            for (int j = 0; j < cols; j++) {
                retval.col(j) = fft.fwd(X.col(j).eval());
            }
        } else {
            for (int i = 0; i < rows; i++) {
                retval.row(i) = fft.fwd(X.row(i).eval());
            }
        }

        return retval;
    }

    /*! @brief 型指定しない場合を担保
    */
    template <int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::complexify <From>::type, Rows, Cols> fft(const Eigen::Matrix <From, Rows, Cols> &X, const int option = Eigen::ColMajor)
    {
        return fft<typename std::complexify<From>::type>(X, option);
    }

    /*! @brief fft<double>の場合を担保
    */
    template <typename To, typename From, int Rows, int Cols, typename std::enable_if <!std::is_same <To, typename std::complexify <To>::type>::value>::type * = nullptr>
    Eigen::Matrix <To, Rows, Cols> fft(const Eigen::Matrix <From, Rows, Cols> &X, const int option = Eigen::ColMajor)
    {
        return fft<typename std::complexify<To>::type>(X, option).real();
    }

    /*! @brief 1変量周波数系列の逆FFT
        @tparam To 実数型または複素数型．
        指定しないと複素数型になる
        @tparam From 複素数型
        @tparam N 系列の長さ
        @param x 1変量周波数系列
    */

    /*! @brief std::array用
    */

    /*! @brief 引数がstd::complex<double>>の場合を担保
    */
    template <typename To, typename From, size_t N, typename std::enable_if <std::is_same <From, typename std::complexify <From>::type>::value>::type * = nullptr>
    std::array <To, N> ifft(const std::array <From, N> &X)
    {
        using result_type = std::array <To, N>;
        result_type                                          retval;
        Eigen::FFT <typename std::decomplexify <From>::type> fft;
        fft.inv(retval.data(), X.data(), N);

        return retval;
    }

    /*! @brief 引数がdoubleの場合を担保
    */
    template <typename To, typename From, size_t N, typename std::enable_if <!std::is_same <From, typename std::complexify <From>::type>::value>::type * = nullptr>
    std::array <To, N> ifft(const std::array <From, N> &X)
    {
        using tmp_type = std::array <typename std::complexify <From>::type, N>;
        tmp_type tmp;
        for (size_t i = 0; i < N; i++) {
            tmp[i].real(X[i]);
            tmp[i].imag(0.);
        }
        return ifft<To>(tmp);
    }

    /*! @brief 値域が指定されていない場合を担保
    */
    template <size_t N, typename From>
    std::array <typename std::complexify<From>::type, N> ifft(const std::array <From, N> &X)
    {
        return ifft<typename std::complexify<From>::type>(X);
    }

    /*! @brief std::vector用
    */

    /*! @brief ifft<double>の場合を担保
    */
    template <typename To, typename From, typename std::enable_if <!std::is_same <To, typename std::complexify <To>::type>::value>::type * = nullptr>
    std::vector <To> ifft(const std::vector <From> &X)
    {
        using result_type = std::vector <To>;
        result_type                                          retval;
        Eigen::FFT <typename std::decomplexify <From>::type> fft;
        fft.inv(retval, X);

        return retval;
    }

    /*! @brief ifft，ifft<std::complex<double>>の場合を担保
    */
    template <typename Complex, typename std::enable_if <std::is_same <Complex, typename std::complexify <Complex>::type>::value>::type * = nullptr>
    std::vector <Complex> ifft(const std::vector <Complex> &X)
    {
        using result_type = std::vector <Complex>;
        result_type                                             retval;
        Eigen::FFT <typename std::decomplexify <Complex>::type> fft;
        fft.inv(retval, X);

        return retval;
    }

    /*! @brief 多変量周波数系列の逆FFT．
        @tparam To 実数型または複素数型．
        指定しないと複素数型になる
        @tparam From 複素数型
        @tparam Rows 行数
        @tparam Cols 列数
        @param X 周波数データ行列
        @param option 時間方向の縦横．デフォルト値はEigen::ColMajorでこのとき時間方向は縦
    */

    /*! @brief Eigen::Matrix用
    */

    /*! @brief 引数がstd::complex<double>>の場合を担保
    */
    template <typename To, typename From, int Rows, int Cols, typename std::enable_if <std::is_same <From, typename std::complexify <From>::type>::value>::type * = nullptr>
    Eigen::Matrix <To, Rows, Cols> ifft(const Eigen::Matrix <From, Rows, Cols> &X, const int option = Eigen::ColMajor)
    {
        using result_type = Eigen::Matrix <To, Rows, Cols>;
        const int                                            rows = X.rows();
        const int                                            cols = X.cols();
        result_type                                          retval(rows, cols);
        Eigen::FFT <typename std::decomplexify <From>::type> fft;
        if (option == Eigen::ColMajor) {
            Eigen::Vector<To, Rows> tmp;
            for (int j = 0; j < cols; j++) {
                fft.inv(tmp, X.col(j).eval());
                retval.col(j) = tmp;
            }
        } else {
            Eigen::RowVector<To, Cols> tmp;
            for (int i = 0; i < rows; i++) {
                fft.inv(tmp, X.row(i).eval());
                retval.row(i) = tmp;
            }
        }

        return retval;
    }

    /*! @brief 引数がdoubleの場合を担保
    */
    template <typename To, typename From, int Rows, int Cols, typename std::enable_if <!std::is_same <From, typename std::complexify <From>::type>::value>::type * = nullptr>
    Eigen::Matrix <To, Rows, Cols> ifft(const Eigen::Matrix <From, Rows, Cols> &X, const int option = Eigen::ColMajor)
    {
        const int                                            rows = X.rows();
        const int                                            cols = X.cols();
        using tmp_type = Eigen::Matrix <typename std::complexify <From>::type, Rows, Cols>;
        tmp_type tmp(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                tmp(i, j).real(X(i, j));
                tmp(i, j).imag(0.);
            }
        }
        return ifft<To>(tmp, option);
    }

    /*! @brief 値域が指定されていない場合を担保
    */
    template <int Rows, int Cols, typename From>
    Eigen::Matrix <typename std::complexify<From>::type, Rows, Cols> ifft(const Eigen::Matrix <From, Rows, Cols> &X, const int option = Eigen::ColMajor)
    {
        return ifft<typename std::complexify<From>::type>(X, option);
    }
}

#endif
