#pragma once

﻿char *gets(char *str);

#include "basefunc.hpp"

namespace symbfunc {

    // シンボリック関数
    template <>
    class Func<0> {
    public:
        Func() = delete; // コンストラクタ
        Func(const Symbol& s); // コンストラクタ
        ~Func(); // デストラクタ
        // コピーコンストラクタ
        // コピー代入演算子
        // ムーブコンストラクタ
        // ムーブ代入演算子

        // 文字列変換
        std::string str() const;

        // 関数適用（未定義の場合がある）
        template <typename _type>
        boost::optional <_type> eval(boost::optional<_type> t, const Symbol& s) const;

        // 微分
        void d(std::vector <std::unique_ptr<Func>> &operand, const Symbol& s) const;

    private:
        std::string _name;
    };
    using symb = Func<0>;

};
