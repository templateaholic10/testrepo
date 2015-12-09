/*! @file
    @brief 関数テンプレートに関する誤解
    @author yanteyon10
    @date 11/29
*/

#include <iostream>

namespace test {
    /*! @brief 関数テンプレートの特殊化
    */
    template <typename To, typename From>
    To convert(const From &from)
    {
        return static_cast<To>(from);
    }
}
