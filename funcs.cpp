#include "funcs.hpp"

namespace symbfunc {

    template <>
    Func<0>::Func(const Symbol& s)
    : _name(s.name)
    {
    }

    // 文字列変換
    template <>
    std::string Func<0>::str()
    {
        return _name;
    }

    // 関数適用（未定義の場合がある）
    template <> template <typename _type>
    boost::optional <_type> Func<0>::eval(boost::optional <_type> t, const Symbol& s) const
    {
        if (s.name == _name) {
            return 1;
        } else {
            return 0;
        }
    }

    // 微分
    template <>
    void Func<0>::d(std::vector <std::unique_ptr<Func>> &operand) const
    {
        return;
    }

}
