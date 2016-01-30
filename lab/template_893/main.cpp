#include <debug>
#include "function.hpp"

int main(int argc, char const *argv[]) {
    const double hoge = 1.2;
    auto piyo = test::convert<int>(hoge);
    _PRINT(piyo)
    _TYPEOF(piyo)
    return 0;
}
