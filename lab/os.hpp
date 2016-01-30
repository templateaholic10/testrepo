/*! @file
    @brief pythonのosモジュールを模倣したユーティリティ
    @auther yanteyon10
    @date 11/3
*/

#ifndef OS_HPP
#define OS_HPP

#include <string>

namespace os {
    namespace path {
        std::string join(const std::string& former, const std::string& latter)
        {
            return former + "/" + latter;
        }
    }
}

#endif
