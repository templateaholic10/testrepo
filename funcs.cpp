#include "funcs.hpp"

namespace symbfunc {

    symb::symb(const Symbol& s)
    : _name(s.name)
    {
    }


    template <int N> power<N>::power(Basefunc f)
    {
        _operand = std::make_unique<Basefunc>();
    }
}
