#include <basefunc.hpp>

namespace symbfunc {

    // 文字列変換
    template <int Index>
    std::string Func<Index>::str() const
    {
        return "";
    }

    // 関数適用（未定義の場合がある）
    // メンバテンプレートは仮想関数にできない（オーバーライドされる）
    template <int Index> template <typename _type>
    boost::optional <_type> Func<Index>::eval(boost::optional <_type> t, const Symbol& s) const
    {
        return t;
    }

    // 微分
    template <int Index>
    void Func<Index>::d(std::vector <std::unique_ptr <Func> > &operand, const Symbol& s) const
    {
        return;
    }

}
