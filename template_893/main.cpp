char *gets(char *str);

#include "template_893.hpp"

int main(int argc, char const *argv[]) {
    printprintf("-", 20); std::cout << std::endl;
    test1_1();
    // printprintf("-", 20);
    // test1_2();  // エラーになるコード
    printprintf("-", 20); std::cout << std::endl;
    test2();
    printprintf("-", 20); std::cout << std::endl;
    return 0;
}
