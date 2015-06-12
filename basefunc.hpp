#pragma once

﻿char *gets(char *str);

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <complex>
#include <boost/optional.hpp>

namespace symbfunc {
    // シンボリッククラス
    struct Symbol {
        std::string name;
    };

    // テンプレートクラス
    template <int Index>
    class Func {
    public:
        Func() = default;     // コンストラクタ
        ~Func() = default;     // デストラクタ
        // コピーコンストラクタ
        // コピー代入演算子
        // ムーブコンストラクタ
        // ムーブ代入演算子

        // 文字列変換
        std::string str() const;

        // 関数適用（未定義の場合がある）
        // メンバテンプレートは仮想関数にできない（オーバーライドされる）
        template <typename _type>
        boost::optional <_type> eval(boost::optional <_type> t, const Symbol& s) const;

        // 微分
        void d(std::vector <std::unique_ptr <Func> > &operand, const Symbol& s) const;

    private:
        std::string _name; // シンボリック保持用
    };
}
