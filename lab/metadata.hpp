#ifndef METADATA
#define METADATA

#include <boost/mpl/vector.hpp>
#include <sprout/string.hpp>

namespace animal {
    struct Animal { };

    struct Human { };

    struct Kameoka { };

    struct Akari { };

    struct non_Kameoka { };

    struct Man { };

    struct Woman { };

    struct Otaku { };

    // 要素メタデータ
    using Elements = boost::mpl::vector <Animal, Human, Kameoka, Akari, non_Kameoka, Man, Woman, Otaku>;

    // 木構造
    constexpr auto p_seq = sprout::to_string("(((())(()()))())");
}

namespace derivation {
    // 要素メタデータ
    using Elements = boost::mpl::vector <int, const int, const int *, int *, int *const, int *const *, int *[], int *&, int&, int[], int(*)[]>;

    // 木構造
    constexpr auto p_seq = sprout::to_string("((())((())()())()(()))");
}

#endif
