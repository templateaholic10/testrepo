/*! @file
    @brief デフォルト値を統一的に扱うインタフェース
    @auther yanteyon10
    @date 11/10
*/

#ifndef DEFAULT_VALUE_HPP
#define DEFAULT_VALUE_HPP

#include <type_traits>

namespace util {
    /*! @brief 型のデフォルト値を返す構造体
        @tparam T デフォルト値を利用したい型
    */
    template <typename T, class Ignored = void>
    struct Default_value;

    template <typename T>
    struct Default_value<T, typename std::enable_if<std::is_integral<T>::value>::type>
    {
        static T eval()
        {
            return 0;
        }
    };

    template <typename T>
    struct Default_value<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
    {
        static T eval()
        {
            return 0.;
        }
    };
}

#ifdef COMPLEX_DEFAULT
#include "_header/default_value_complex.hpp"
#endif

#ifdef EIGEN_MATRIX_DEFAULT
#include "_header/default_value_eigen_matrix.hpp"
#endif

namespace util {
    /*! @brief 型のデフォルト値を返す関数
        @tparam T デフォルト値を利用したい型
    */
    template <typename T>
    T default_value()
    {
        return Default_value<T>::eval();
    }
}

#endif
