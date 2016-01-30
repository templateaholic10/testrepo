/*! @file
    @brief type_traitsのユーティリティメタ関数
*/

#include <type_traits>

namespace std {
    /*! @brief 順序関係が定義されない場合
    */
    template <typename T1, typename T2, typename Ignored1 = void, typename Ignored2 = void>
    struct join;

    /*! ともに整数型
    */
    template <typename T1, typename T2>
    struct join <T1, T2, typename std::enable_if<std::is_integral<T1>::value>::type, typename std::enable_if<std::is_integral<T2>::value>::type> {
        using type = T1;
    };

    /*! 整数型，実数型
    */
    template <typename T1, typename T2>
    struct join <T1, T2, typename std::enable_if<std::is_integral<T1>::value>::type, typename std::enable_if<std::is_floating_point<T2>::value>::type> {
        using type = T2;
    };

    /*! 実数型，整数型
    */
    template <typename T1, typename T2>
    struct join <T1, T2, typename std::enable_if<std::is_floating_point<T1>::value>::type, typename std::enable_if<std::is_integral<T2>::value>::type> {
        using type = T1;
    };

    /*! ともに実数型
    */
    template <typename T1, typename T2>
    struct join <T1, T2, typename std::enable_if<std::is_floating_point<T1>::value>::type, typename std::enable_if<std::is_floating_point<T2>::value>::type> {
        using type = T1;
    };
}
