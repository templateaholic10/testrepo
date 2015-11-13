/*! @file
    @brief 零元，単位元，デフォルト値を統一的に扱うインタフェース
    @auther yanteyon10
    @date 11/10
*/

#ifndef ZERO_ONE_HPP
#define ZERO_ONE_HPP

#include <type_traits>

namespace util {
    /*! @brief 型の零元を返す構造体
        @tparam T 零元を利用したい型
    */
    template <typename T, class Ignored = void>
    struct Zero;

    template <typename T>
    struct Zero<T, typename std::enable_if<std::is_integral<T>::value>::type>
    {
        static T eval()
        {
            return 0;
        }
    };

    template <typename T>
    struct Zero<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
    {
        static T eval()
        {
            return 0.;
        }
    };

    /*! @brief 型の乗法単位元を返す構造体
        @tparam T 乗法単位元を利用したい型
    */
    template <typename T, class Ignored = void>
    struct One;

    template <typename T>
    struct One<T, typename std::enable_if<std::is_integral<T>::value>::type>
    {
        static T eval()
        {
            return 1;
        }
    };

    template <typename T>
    struct One<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
    {
        static T eval()
        {
            return 1.;
        }
    };
}

#ifdef COMPLEX_ZERO_ONE
#include "_header/zero_one_complex.hpp"
#endif

#ifdef EIGEN_MATRIX_ZERO_ONE
#include "_header/zero_one_eigen_matrix.hpp"
#endif

namespace util {
    template <typename T>
    using Default_value = Zero<T>;
}

namespace util {
    /*! @brief 型の零元を返す関数
        @tparam T 零元を利用したい型
    */
    template <typename T>
    T zero()
    {
        return Zero<T>::eval();
    }

    /*! @brief 型の乗法単位元を返す関数
        @tparam T 乗法単位元を利用したい型
    */
    template <typename T>
    T one()
    {
        return One<T>::eval();
    }

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
