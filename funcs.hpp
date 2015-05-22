#pragma once

﻿char *gets(char *str);

#include "basefunc.hpp"

namespace symbfunc {

    class symb : Basefunc {
    public:
        symb() = delete; // コンストラクタ
        symb(const Symbol& s); // コンストラクタ
        ~symb(); // デストラクタ
        // コピーコンストラクタ
        // コピー代入演算子
        // ムーブコンストラクタ
        // ムーブ代入演算子

        // 文字列変換
        std::string str() const;

        // 関数適用（未定義の場合がある）
        template <typename _type>
        boost::optional <_type> eval(boost::optional <_type> t) const;

        // 微分
        void d(std::vector <std::unique_ptr <Basefunc> > &operand) const;

    private:
        std::string _name;
    }

    template <int N>
    class power : Basefunc {
    public:
        power() = delete; // コンストラクタ
        power(Basefunc f); // コンストラクタ
        ~power(); // デストラクタ
        // コピーコンストラクタ
        // コピー代入演算子
        // ムーブコンストラクタ
        // ムーブ代入演算子

        // 文字列変換
        std::string str() const;

        // 関数適用（未定義の場合がある）
        template <typename _type>
        boost::optional <_type> eval(boost::optional <_type> t) const;

        // 微分
        void d(std::vector <std::unique_ptr <Basefunc> > &operand) const;

    private:
        std::unique_ptr <Basefunc> _operand;
    };

}
