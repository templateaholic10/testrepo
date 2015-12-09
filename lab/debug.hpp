﻿/*! @file
    @brief デバッグ用マクロ群
    @author yanteyon10
    @date 11/29
*/

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include <boost/type.hpp>

/*! @macro
    @brief 行位置マーカー
*/
#ifdef _HERE
    #undef _HERE
#endif
#define _HERE { std::cout << __LINE__ << std::endl; }

/*! @macro
    @brief オブジェクトプリンタ
*/
#ifdef _PRINT
    #undef _PRINT
#endif
#define _PRINT(var) { std::cout << "$" #var ": " << (var) << std::endl; }

/*! @macro
    @brief シーケンスプリンタ
*/
#ifdef _PRINTSEQ
    #undef _PRINTSEQ
#endif
#define _PRINTSEQ(seq)                   \
    {                                    \
        std::cout << "$" #seq ":";       \
        for (const auto &elem : (seq)) { \
            std::cout << " " << elem;    \
        }                                \
        std::cout << std::endl;          \
    }

/*! @macro
    @brief 型名プリンタ
*/
#ifdef _TYPE
    #undef _TYPE
#endif
#define _TYPE(var) { std::cout << "$" #var ": " << util::typename_of <var>() << std::endl; }

/*! @macro
    @brief オブジェクトの型名プリンタ．参照型を区別しない
*/
#ifdef _TYPEOF
    #undef _TYPEOF
#endif
#define _TYPEOF(var) { std::cout << "$" #var ": " << _debug_hpp::type <decltype(var)>() << std::endl; }

/*! @macro
    @brief オブジェクトの型名プリンタ．参照型を区別する
*/
#ifdef _DETAILTYPEOF
    #undef _DETAILTYPEOF
#endif
#define _DETAILTYPEOF(var) { std::cout << "$" #var ": " << _debug_hpp::detailtype <decltype(var)>() << std::endl; }

/*! @namespace _debug_hpp
    @brief マクロのための内部関数．
    野良C++erの雑記帳（http://d.hatena.ne.jp/gintenlabo/20100116/1263681145）様から
*/
namespace _debug_hpp {
    /*! @brief typeinfoのデマングル関数．メモリリークが存在
    */
    char *demangle(const char *demangled)
    {
        int status;
        return abi::__cxa_demangle(demangled, 0, 0, &status);
    }

    /*! @brief 型名を返す関数テンプレート．参照型を区別しない
    */
    template <typename T>
    char *type()
    {
        return demangle(typeid(T).name() );
    }

    /*! @brief 型名を返す関数テンプレート．参照型を区別する
    */
    template <typename T>
    char *detailtype()
    {
        return demangle(typeid(boost::type <T> ).name() );
    }
}

#endif
