char *gets(char *str);

#include "template_893.hpp"
#include "../util.hpp"

int main(int argc, char const *argv[]) {
    using namespace util;
    
    auto horizonline = [](){repeat(std::cout, "-", 20); std::cout << std::endl;};

    horizonline();
    ex1::test1();
    // horizonline();
    // ex1::test1_2();  // エラーになるコード
    horizonline();
    ex2::test2();
    horizonline();
    ex3::test3();
    horizonline();
    return 0;
}