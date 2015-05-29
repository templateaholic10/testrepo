char *gets(char *str);
#include <iostream>
#include <boost/type_traits/is_integral.hpp>

template <typename _type>
bool Is_int(_type T) {
    return boost::is_integral<_type>::type::value;
}

int main(int argc, char const *argv[]) {
    int a = 0;
    char c = 0;
    double d = 0;

    std::cout << "a is " << ((Is_int(a)) ? "integer" : "not integer") << std::endl;
    std::cout << "c is " << ((Is_int(c)) ? "integer" : "not integer") << std::endl;
    std::cout << "d is " << ((Is_int(d)) ? "integer" : "not integer") << std::endl;
    return 0;
}
