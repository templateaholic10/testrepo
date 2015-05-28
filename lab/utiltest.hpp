#ifndef UTILTEST
#define UTILTEST

#include "util.hpp"

namespace util {

    template <class T, std::size_t Size>
    void disp_array(const std::array<T, Size>& A)
    {
        for (auto it = A.begin(); it != A.end(); ++it) {
            std::cout << *it << ((it != A.end() - 1) ? ' ' : '\n');
        }
    }

    // apply関数のテスト
    void testapply()
    {
        // const left，1次元

        // リテラル型のarrayはコンパイル時定数にできる．
        constexpr auto x =  make_array<int>(1, 2, 3, 4);
        disp_array(x);
        // arrayをインクリメント．
        const auto increment = [=](int i){return i + 1;};
        // applyの返り値でarrayを初期化．
        auto apx = apply(x, increment);
        disp_array(apx);

        // リテラル型でないarrayはコンパイル時定数にできない．
        const auto y =  make_array<std::string>("Ypa", "Oi", "Way", "So", "Yeah");
        disp_array(y);
        // arrayをあかりちゃんから引用する．
        const auto quote = [&](const std::string& str){return str + " by Akari Ozora";};
        // applyの返り値でarrayを初期化．
        auto apy = apply(y, quote);
        disp_array(apy);

        // auto apy_error = apply(y, increment);  // これはコンパイルエラー．静的な型チェックOK．

        // 再代入も可能．
        auto z = make_array<double>(3.1415, 2.71828, 0.57721);
        disp_array(z);
        // arrayをインクリメント．
        const auto twice = [=](double d){return d * 2.;};
        // 再代入．
        z = apply(z, twice);
        disp_array(z);

        return;
    }
}

#endif
