/*! @file
    @brief 種々のテスト関数
    @auther yanteyon10
    @date 11/21
*/

#ifndef TESTFUNCTIONS_HPP
#define TESTFUNCTIONS_HPP

#include <iostream>
#include <functional>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

namespace test {
    /*! @brief 通常の関数
    */
    template <typename T>
    T id(const T &x)
    {
        return x;
    }

    /*! @brief 関数オブジェクト
    */
    template <class T>
    struct Add {
        T operator()(const T &x, const T &y)
        {
            return x + y;
        }
    };

    template <class T>
    struct Addconst {
    private:
        T c;
    public:
        Addconst(const T &c_)
            : c(c_)
        {
        }

        T operator()(const T &x)
        {
            return x + c;
        }
    };

    #define MAX_SIZE 50
    template <typename T>
    T project(const int place, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(MAX_SIZE, const T& x, 0))
    {
        #define COND(z, it, data) if (place == it) { \
            return x ## it; \
        }
        BOOST_PP_REPEAT(MAX_SIZE, COND, )
        else {
            return 0;
        }
    }
    #undef COND
    #undef MAX_SIZE

    /*! @brief ラムダ式
    */
    auto horizonline = [](const int n) {
                           for (int i = 0; i < n; i++) {
                               std::cout << '-' << std::endl;
                           }
                       };

    /*! @brief メンバ関数
    */
}

#endif
