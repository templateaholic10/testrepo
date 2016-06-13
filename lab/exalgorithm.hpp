/*! @file
    @brief algorithmヘッダの拡張
    @auther templateaholic10
    @date 12/30
*/

#ifndef EXALGORITHM_HPP
#define EXALGORITHM_HPP

#include <algorithm>

namespace std {
    template<class InputIt1, class InputIt2, class BinaryFunction>
    BinaryFunction for_each_impl(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryFunction f, std::input_iterator_tag, std::input_iterator_tag)
    {
        for (; first1 != last1; ++first1, ++first2) {
            f(*first1, *first2);
        }
        return f;
    }

    template<class InputIt1, class InputIt2, class BinaryFunction>
    BinaryFunction for_each(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryFunction f)
    {
        using Category1 = typename std::iterator_traits<InputIt1>::iterator_category;
        using Category2 = typename std::iterator_traits<InputIt2>::iterator_category;
        return for_each_impl(first1, last1, first2, f, Category1(), Category2());
    }
}

#endif
