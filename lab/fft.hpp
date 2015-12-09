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

namespace Eigen {
    /*! @brief 1変量時系列のFFT．
        時間領域では実数または複素数型，周波数領域では複素数型なので指定する必要はない
        @tparam T 実数型または複素数型
        @tparam N 系列の長さ
        @param x 1変量時系列
    */

    /*! @brief std::array用
    */
    template <typename T, size_t N>
    std::array <typename std::complexify <T>::type, N> fft(const std::array <T, N> &x)
    {
        using result_type = std::array <typename std::complexify <T>::type, N>;
        result_type                                       retval;
        Eigen::FFT <typename std::decomplexify <T>::type> fft;
        fft.fwd(retval.data(), x.data(), N);

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
        @tparam T 実数型または複素数型
        @tparam Rows 行数
        @tparam Cols 列数
        @param X データ行列
        @param transpose 通常のデータ行列（時間方向が行方向）から転置する
        デフォルト値はfalse
    */

    /*! @brief Eigen::Matrix用
    */
    template <typename T, int Rows, int Cols>
    Eigen::Matrix <typename std::complexify <T>::type, Rows, Cols> fft(const Eigen::Matrix <T, Rows, Cols> &X, const bool transpose=false)
    {
        using result_type = Eigen::Matrix <typename std::complexify <T>::type, Rows, Cols>;
        const int                                         rows = X.rows();
        const int                                         cols = X.cols();
        result_type                                       retval(rows, cols);
        Eigen::FFT <typename std::decomplexify <T>::type> fft;
        if (transpose) {
            for (int i = 0; i < rows; i++) {
                retval.row(i) = fft.fwd(X.row(i).eval());
            }
        } else {
            for (int j = 0; j < cols; j++) {
                retval.col(j) = fft.fwd(X.col(j).eval());
            }
        }

        return retval;
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

    /*! @brief ifft<double>の場合を担保
    */
    template <typename To, typename From, size_t N, typename std::enable_if <!std::is_same <To, typename std::complexify <To>::type>::value>::type * = nullptr>
    std::array <To, N> ifft(const std::array <From, N> &X)
    {
        using result_type = std::array <To, N>;
        result_type                                          retval;
        Eigen::FFT <typename std::decomplexify <From>::type> fft;
        fft.inv(retval.data(), X.data(), N);

        return retval;
    }

    /*! @brief ifft，ifft<std::complex<double>>の場合を担保
    */
    template <typename Complex, size_t N, typename std::enable_if <std::is_same <Complex, typename std::complexify <Complex>::type>::value>::type * = nullptr>
    std::array <Complex, N> ifft(const std::array <Complex, N> &X)
    {
        using result_type = std::array <Complex, N>;
        result_type                                             retval;
        Eigen::FFT <typename std::decomplexify <Complex>::type> fft;
        fft.inv(retval.data(), X.data(), N);

        return retval;
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
        @param transpose 通常のデータ行列（時間方向が行方向）から転置する
        デフォルト値はfalse
    */

    /*! @brief Eigen::Matrix用
    */

    /*! @brief ifft<double>の場合を担保
    */
    template <typename To, typename From, int Rows, int Cols, typename std::enable_if <!std::is_same <To, typename std::complexify <To>::type>::value>::type * = nullptr>
    Eigen::Matrix <To, Rows, Cols> ifft(const Eigen::Matrix <From, Rows, Cols> &X, const bool transpose=false)
    {
        using result_type = Eigen::Matrix <To, Rows, Cols>;
        const int                                            rows = X.rows();
        const int                                            cols = X.cols();
        result_type                                          retval(rows, cols);
        Eigen::FFT <typename std::decomplexify <From>::type> fft;
        if (transpose) {
            Eigen::RowVector<To, Cols> tmp;
            for (int i = 0; i < rows; i++) {
                fft.inv(tmp, X.row(i).eval());
                retval.row(i) = tmp;
            }
        } else {
            Eigen::Vector<To, Rows> tmp;
            for (int j = 0; j < cols; j++) {
                fft.inv(tmp, X.col(j).eval());
                retval.col(j) = tmp;
            }
        }

        return retval;
    }

    /*! @brief ifft，ifft<std::complex<double>>の場合を担保
    */
    template <typename Complex, int Rows, int Cols, typename std::enable_if <std::is_same <Complex, typename std::complexify <Complex>::type>::value>::type * = nullptr>
    Eigen::Matrix <Complex, Rows, Cols> ifft(const Eigen::Matrix <Complex, Rows, Cols> &X, const bool transpose=false)
    {
        using result_type = Eigen::Matrix <Complex, Rows, Cols>;
        const int                                            rows = X.rows();
        const int                                            cols = X.cols();
        result_type                                          retval(rows, cols);
        Eigen::FFT <typename std::decomplexify <Complex>::type> fft;
        if (transpose) {
            for (int i = 0; i < rows; i++) {
                retval.row(i) = fft.inv(X.row(i).eval());
            }
        } else {
            for (int j = 0; j < cols; j++) {
                retval.col(j) = fft.inv(X.col(j).eval());
            }
        }

        return retval;
    }
}

#endif
