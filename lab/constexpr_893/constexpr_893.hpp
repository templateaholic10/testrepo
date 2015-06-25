#ifndef CONSTEXPR_893
#define CONSTEXPR_893

#include "../util.hpp"

namespace constexpr_893 {
    // コンパイル時構築可能なユーザ定義クラス（リテラルクラス）の定義．
    class Simple_class
    {
    public:
        constexpr Simple_class()
        : _birthdays(util::constexpr_array<int, 3>())
        {_birthdays[0] = 4;}
        constexpr int akari() const
        {return _birthdays[0];}
    private:
        util::constexpr_array<int, 3> _birthdays;
    };

    void testSimple_class()
    {
        constexpr Simple_class sc;
        constexpr auto akari = sc.akari();
        std::cout << akari << std::endl;
        unsigned long honya = 981820850679;
        std::cout << "unsigned char: " << static_cast<unsigned char>(honya) << std::endl;
        std::cout << "unsigned short: " << static_cast<unsigned char>(honya) << std::endl;
        std::cout << "unsigned int: " << static_cast<unsigned char>(honya) << std::endl;
        std::cout << "unsigned long: " << static_cast<unsigned char>(honya) << std::endl;
    }
}

#endif
