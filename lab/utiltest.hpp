#ifndef UTILTEST
#define UTILTEST

#include "util.hpp"

namespace util {
    template <class T, std::size_t Size>
    void disp_array(const std::array <T, Size> &A)
    {
        for (auto it = A.begin(); it != A.end(); ++it) {
            std::cout << *it << ((it != A.end() - 1) ? ' ' : '\n');
        }
    }

    // convert_arrayメタ関数のテスト
    void testconvert_array()
    {
        std::cout << "test convert_array : " << std::endl;
        constexpr convert_array<std::array<std::array<int, 3>, 2>, double, 2>::type y = make_common_array(make_common_array(0., 1., 2.), make_common_array(3., 4., 5.));
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                std::cout << y[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    enum class APPLYTYPE { CONST_L_1, CONST_L_N, L_1, L_N, R_1, R_N };

    // apply関数のテスト
    template <APPLYTYPE applytype>
    void testapply_sub()
    {
    }

    template <>
    void testapply_sub <APPLYTYPE::CONST_L_1>()
    {
        // const lvalue，1次元

        // リテラル型のarrayはコンパイル時定数にできる．
        constexpr auto x =  make_array <int>(1, 2, 3, 4);
        disp_array(x);
        // arrayをインクリメント．
        const auto increment = [ = ](int i) {
                                   return i + 1;
                               };
        // applyの返り値でarrayを初期化．
        auto apx = apply(x, increment);
        disp_array(apx);

        // リテラル型でないarrayはコンパイル時定数にできない．
        const auto y =  make_array <std::string>("Ypa", "Oi", "Way", "So", "Yeah");
        disp_array(y);
        // arrayをあかりちゃんから引用する．
        const auto quote = [&](const std::string &str) {
                               return str + " by Akari Ozora";
                           };
        // applyの返り値でarrayを初期化．
        auto apy = apply(y, quote);
        disp_array(apy);

        // auto apy_error = apply(y, increment);  // これはコンパイルエラー．静的な型チェックOK．

        // 再代入も可能．
        auto z = make_array <double>(3.1415, 2.71828, 0.57721);
        disp_array(z);
        // arrayを2倍．
        const auto twice = [ = ](double d) {
                               return d * 2.;
                           };
        // 再代入．
        z = apply(z, twice);
        disp_array(z);
    }

    template <>
    void testapply_sub <APPLYTYPE::CONST_L_N>()
    {
        // const lvalue，n次元
        /*
                // リテラル型のarrayはコンパイル時定数にできる．
                constexpr multi_array<double, 2, 3> x =  make_common_array(make_common_array(0.0, 3.6, 2.6), make_common_array(9.5, 3.8, 9.3));
                // arrayを2倍．
                const auto twice = [ = ](double d) {
                                       return d * 2.;
                                   };
                // applyの返り値でarrayを初期化．
                auto apx = apply(x, twice);
                disp_array(apx);
        */
        /*
                // リテラル型でないarrayはコンパイル時定数にできない．
                const auto y =  make_array <std::string>("Ypa", "Oi", "Way", "So", "Yeah");
                disp_array(y);
                // arrayをあかりちゃんから引用する．
                const auto quote = [&](const std::string &str) {
                                       return str + " by Akari Ozora";
                                   };
                // applyの返り値でarrayを初期化．
                auto apy = apply(y, quote);
                disp_array(apy);

                // auto apy_error = apply(y, increment);  // これはコンパイルエラー．静的な型チェックOK．

                // 再代入も可能．
                auto z = make_array <double>(3.1415, 2.71828, 0.57721);
                disp_array(z);
                // arrayを2倍．
                const auto twice = [ = ](double d) {
                                       return d * 2.;
                                   };
                // 再代入．
                z = apply(z, twice);
                disp_array(z);
                */
    }

    template <>
    void testapply_sub <APPLYTYPE::R_1>()
    {
        // rvalue，1次元

        // インクリメント関数の定義．
        const auto increment = [ = ](int i) {
                                   return i + 1;
                               };
        // applyの返り値でarrayを初期化．
        auto x = apply(make_array <int>(1, 2, 3, 4), increment);
        disp_array(x);
    }

    template <>
    void testapply_sub <APPLYTYPE::L_1>()
    {
        // lvalue，1次元

        auto x = make_array <int>(1, 2, 3, 4);
        // インクリメント関数の定義．
        const auto outInt = [ = ](int i) {
                                std::cout << i;
                            };
        apply_void(x, outInt);
        std::cout << std::endl;
        // 副作用を持つtwice関数の定義
        const auto twice = [](int &i){i *= 2;};
        apply_void(x, twice);
        apply_void(x, outInt);
        std::cout << std::endl;
    }

    void testapply()
    {
        testapply_sub <APPLYTYPE::CONST_L_1>();
        testapply_sub <APPLYTYPE::L_1>();
        testapply_sub <APPLYTYPE::R_1>();
    }
}

#endif
