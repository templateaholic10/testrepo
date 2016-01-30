/*! @file
    @brief std::arrayを拡張するクラス
    @author yanteyon10
    @date 11/18
*/
#ifndef ARRAY_MATRIX_HPP
#define ARRAY_MATRIX_HPP

#include <iostream>
#include <array>
#include <cstdlib>

namespace std {
    /*! @class
        @brief 2次元std::arrayを拡張するstd::matrixクラス．入出力を備えただけ
    */
    template <typename T, size_t m, size_t n>
    class matrix;

    template <typename T, size_t m, size_t n>
    class matrix : public std::array<std::array<T, n>, m> {
    public:
        static char delim;
    public:
        template <typename T_, size_t m_, size_t n_>
        friend std::ostream& operator<<(std::ostream& os, const matrix<T_, m_, n_>& M);
        template <typename T_, size_t m_, size_t n_>
        friend std::istream& operator>>(std::istream& is, matrix<T_, m_, n_>& M);
    };

    template <typename T, size_t m, size_t n>
    char matrix<T, m, n>::delim = ',';

    template <typename T, size_t m, size_t n>
    std::ostream& operator<<(std::ostream& os, const matrix<T, m, n>& M)
    {
        if (m == 0 || n == 0) {
            return os;
        }
        os << M[0][0];
        for (int j = 1; j < n; j++) {
            os << matrix<T, m, n>::delim << M[0][j];
        }
        for (int i = 1; i < m; i++) {
            os << std::endl << M[i][0];
            for (int j = 1; j < n; j++) {
                os << matrix<T, m, n>::delim << M[i][j];
            }
        }
        return os;
    }

    template <typename T, size_t m, size_t n>
    std::istream& operator>>(std::istream& is, matrix<T, m, n>& M)
    {
        for (auto row : M) {
            for (auto elem : row) {
                is >> elem;
                is.ignore(1, matrix<T, m, n>::delim);
            }
        }
        return is;
    }
}

#endif
