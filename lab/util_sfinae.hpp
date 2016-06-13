/*! @file
    @brief SFINAEのユーティリティメタ関数群
    @auther templateaholic10
    @date 11/19
*/

#ifndef UTIL_SFINAE_HPP
#define UTIL_SFINAE_HPP

namespace util {
    /*! @brief パラメータを評価するメタ関数
    クラスSFINAEにおいてある名前のメンバが存在するかどうかを判定する
    */
    template <class T>
    struct eval {
        using type = void;
    };

    /*! @brief イテレータが定義されているかSFINAEによって確認するメタ関数
    */
    template <class Seq, typename Ignored = void>
    struct has_iterator {
        static constexpr bool value = false;
    };

    template <class Seq>
    struct has_iterator<Seq, typename eval<typename Seq::iterator>::type> {
        static constexpr bool value = true;
    };
}

#endif
