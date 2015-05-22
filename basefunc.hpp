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

    // 基底クラス
    class Basefunc {
    public:
        Basefunc();     // コンストラクタ
        ~Basefunc();     // デストラクタ
        // コピーコンストラクタ
        // コピー代入演算子
        // ムーブコンストラクタ
        // ムーブ代入演算子

        // 文字列変換
        virtual std::string str() const = 0;

        // 関数適用（未定義の場合がある）
        // メンバテンプレートは仮想関数にできない（オーバーライドされる）
        template <typename _type>
        boost::optional <_type> eval(boost::optional <_type> t) const;

        // 微分
        virtual void d(std::vector <std::unique_ptr <Basefunc> > &operand) const = 0;

    private:
    };
}
