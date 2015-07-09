#ifndef UTILTEST
#define UTILTEST

#include "util.hpp"
#include "util_array.hpp"

namespace util {
    template <class T, std::size_t Size>
    void disp_array(const std::array <T, Size> &A)
    {
        for (auto it = A.begin(); it != A.end(); ++it) {
            std::cout << *it << ((it != A.end() - 1) ? ' ' : '\n');
        }
    }

    // nresult_ofメタ関数のテスト
    void testnresult_of()
    {
        std::cout << "test nresult_of : " << std::endl;
        auto add = [](int a, int b) -> double {return static_cast<double>(a+b);};
        auto itos = [](int a) -> std::string {return std::to_string(a);};
        static_assert(std::is_same<nresult_of_t<decltype(itos), int, 1>, std::string>::value, "nresult_of error!");
        // static_assert(std::is_same<nresult_of_t<decltype(add), int, 2>, double>::value, "nresult_of error!");
    }

    // convert_arrayメタ関数のテスト
    void testconvert_array()
    {
        std::cout << "test convert_array : " << std::endl;
        constexpr convert_array <std::array <std::array <int, 3>, 2>, double, 2>::type y = make_common_array(make_common_array(0., 1., 2.), make_common_array(3., 4., 5.));
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                std::cout << y[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    enum class APPLYTYPE { L_1, L_N, R_1, R_N };

    // apply関数のテスト
    template <APPLYTYPE applytype>
    void testapply_sub()
    {
    }

    template <>
    void testapply_sub <APPLYTYPE::L_1>()
    {
        // const lvalue，1次元
        std::cout << "test const lvalue, 1 dim :" << std::endl;

        // リテラル型のarrayはコンパイル時定数にできる．
        constexpr auto x =  make_array <int>(1, 2, 3, 4);
        disp_array(x);
        // arrayをインクリメント．
        const auto increment = [](int i) {
                                   return i + 1;
                               };
        // applyの返り値でarrayを初期化．
        auto apx = Apply <int, 1>::apply(x, increment);
        disp_array(apx);

        // リテラル型でないarrayはコンパイル時定数にできない．
        const auto y =  make_array <std::string>("Ypa", "Oi", "Way", "So", "Yeah");
        disp_array(y);
        // arrayをあかりちゃんから引用する．
        const auto quote = [](const std::string &str) {
                               return str + " by Akari Ozora";
                           };
        // applyの返り値でarrayを初期化．
        auto apy = Apply <std::string, 1>::apply(y, quote);
        disp_array(apy);

        // auto apy_error = apply(y, increment);  // これはコンパイルエラー．静的な型チェックOK．

        // 再代入も可能．
        auto z = make_array <double>(3.1415, 2.71828, 0.57721);
        disp_array(z);
        // arrayを2倍．
        const auto twice = [](double d) {
                               return d * 2.;
                           };
        // 再代入．
        z = Apply <double, 1>::apply(z, twice);
        disp_array(z);

        // voidを返す関数については使えない．
        auto msg      = make_array <std::string>("void", "function", "operate", "this");
        auto printStr = [](const std::string &str) {
                            std::cout << str << ' ';
                        };
        Apply <std::string, 1, void>::apply(msg, printStr);
        std::cout << std::endl;
    }

    template <>
    void testapply_sub <APPLYTYPE::L_N>()
    {
        // const lvalue，n次元
        std::cout << "test const lvalue, n dim :" << std::endl;

        // リテラル型のarrayはコンパイル時定数にできる．
        constexpr multi_array <double, 2, 3> x =  make_common_array(make_common_array(0.0, 3.6, 2.6), make_common_array(9.5, 3.8, 9.3));
        for (int i = 0; i < 2; i++) {
            disp_array(x[i]);
        }
        // arrayを2倍．
        const auto twice = [](double d) {
                               return d * 2.;
                           };
        // applyの返り値でarrayを初期化．
        auto apx = Apply <double, 2>::apply(x, twice);
        for (int i = 0; i < 2; i++) {
            disp_array(apx[i]);
        }
        // std::stringはstd::arrayをまたぐとchar *まわりとの暗黙な変換が行われないので注意する必要がある．
        // 以下のようにするとOK．

        // リテラル型でないarrayはコンパイル時定数にできない．
        auto y = make_common_array(make_array <std::string>("てか", "あかり", "…"), make_array <std::string>("あかり", "〜〜〜〜", "！！！"));
        // 数え上げる関数．
        int        count     = 0;
        const auto Enumerate = [&](const std::string &str) {
                                   count++; std::cout << count << " : " << str << std::endl;
                               };
        Apply <std::string, 2, void>::apply(y, Enumerate);
        // あかり…．
        const auto Quote = [](const std::string &str) -> std::string {
                               return str + "…";
                           };
        // 再代入．
        y = Apply <std::string, 2>::apply(y, Quote);
        Apply <std::string, 2, void>::apply(y, Enumerate);

        // auto apy_error = apply(y, increment);  // これはコンパイルエラー．静的な型チェックOK．

        Apply <std::string, 2, void>::apply(y, Enumerate);

        // lvalue，n次元
        std::cout << "test lvalue, n dim :" << std::endl;

        const auto Encode = [](std::string &str) {
                                for (int i = 0; i < str.length(); i++) { str[i]++; }
                            };

        Apply <std::string, 2, void>::apply_side_effect(y, Encode);
        Apply <std::string, 2, void>::apply(y, Enumerate);
        // Apply<std::string, 2, void>::apply(y, Encode);
    }

    template <>
    void testapply_sub <APPLYTYPE::R_1>()
    {
        // rvalue，1次元
        std::cout << "test rvalue, 1 dim :" << std::endl;

        // インクリメント関数の定義．
        const auto increment = [](int i) {
                                   return i + 1;
                               };
        // applyの返り値でarrayを初期化．
        auto x = Apply <int, 1>::apply(make_array <int>(1, 2, 3, 4), increment);
        disp_array(x);
    }

    template <>
    void testapply_sub <APPLYTYPE::R_N>()
    {
        // rvalue，n次元
        std::cout << "test rvalue, n dim :" << std::endl;

        // arrayを2倍．
        const auto twice = [](double d) {
                               return d * 2.;
                           };
        // applyを2倍したものでarrayを初期化．
        auto apx = Apply <double, 2>::apply(make_common_array(make_common_array(0.0, 3.6, 2.6), make_common_array(9.5, 3.8, 9.3)), twice);
        for (int i = 0; i < 2; i++) {
            disp_array(apx[i]);
        }
    }

    // template <>
    // void testapply_sub <APPLYTYPE::L_1>()
    // {
    //     // lvalue，1次元
    //
    //     auto x = make_array <int>(1, 2, 3, 4);
    //     // インクリメント関数の定義．
    //     const auto outInt = [ = ](int i) {
    //                             std::cout << i;
    //                         };
    //     apply_void(x, outInt);
    //     std::cout << std::endl;
    //     // 副作用を持つtwice関数の定義
    //     const auto twice = [](int &i){i *= 2;};
    //     apply_void(x, twice);
    //     apply_void(x, outInt);
    //     std::cout << std::endl;
    // }

    void testapply()
    {
        auto horizonline = []() {
                               repeat(std::cout, "-", 20); std::cout << std::endl;
                           };

        horizonline();
        horizonline();
        std::cout << "test apply :" << std::endl;
        horizonline();
        horizonline();
        testapply_sub <APPLYTYPE::L_1>();
        horizonline();
        testapply_sub <APPLYTYPE::L_N>();
        horizonline();
        testapply_sub <APPLYTYPE::R_1>();
        horizonline();
        testapply_sub <APPLYTYPE::R_N>();
        horizonline();
        horizonline();
    }
}

#endif
