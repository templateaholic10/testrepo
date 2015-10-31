#ifndef UTIL
#define UTIL

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
// #include <array>
#include <vector>
// #include <utility>
#include <cstddef>
#include <tuple>
#include <map>
#include <bitset>
// #include <boost/type.hpp>
#include <boost/optional.hpp>
#include <typeinfo>
#include <cxxabi.h>

namespace util {
    // ・_HERE関数
    // デバッグ用なのでアンダーバー．
    #ifdef _HERE
    #undef _HERE
    #endif
#define _HERE { std::cout << __LINE__ << std::endl; }

    // ・_DISPLAY関数
    // デバッグ用なのでアンダーバー．
    #ifdef _DISPLAY
    #undef _DISPLAY
    #endif
#define _DISPLAY(var) { std::cout << "$" #var ": " << var << std::endl; }
    #ifdef _DISPLAY_SEQ
    #undef _DISPLAY_SEQ
    #endif
#define _DISPLAY_SEQ(seq)             \
    {                                 \
        std::cout << "$" #seq ":";    \
        for (auto elem : seq) {       \
            std::cout << " " << elem; \
        }                             \
        std::cout << std::endl;       \
    }

    // ・_TYPE関数
    // デバッグ用なのでアンダーバー．
    #ifdef _TYPE
    #undef _TYPE
    #endif
#define _TYPE(var) { std::cout << "$" #var ": " << typename_of <decltype(var)>() << std::endl; }

    /*! @brief 大きい方を返す関数
    */
    template <typename T>
    constexpr T max(T x, T y)
    {
        return (x > y) ? x : y;
    }

    /*! @brief 小さい方を返す関数
    */
    template <typename T>
    constexpr T min(T x, T y)
    {
        return (x < y) ? x : y;
    }

    /*! @brief 均一なarrayを生成する関数
    */
    template <typename T, size_t n>
    std::array<T, n> uniform_array(const T& x)
    {
        std::array<T, n> retval;
        for (auto elem : retval) {
            elem = x;
        }
        return retval;
    }

    // ・repeat関数
    // 文字列strをdelimで区切ってn回osに出力する．
    // 下のRepeatクラスの使用を推奨．
    void repeat(std::ostream &os, const std::string &str, int n)
    {
        for (int i = 0; i < n; i++) {
            os << str;
        }
    }

    void repeat(std::ostream &os, const std::string &str, int n, char delim)
    {
        for (int i = 0; i < n - 1; i++) {
            os << str << delim;
        }
        os << str;
    }

    // ・Repeatクラス
    // std::cout << Repeat("gfn", "2") << std::endl;
    // のように使う．
    // 記法がスマートでよい．
    class Repeat
    {
    public:
        Repeat(const std::string &str, const int n, const char delim='\0');
        friend std::ostream&operator<<(std::ostream &os, const Repeat &rep);

    private:
        const std::string _str;
        const int         _n;
        const char        _delim;
    };

    Repeat::Repeat(const std::string &str, const int n, const char delim)
        : _str(str), _n(n), _delim(delim)
    {
    }

    std::ostream&operator<<(std::ostream &os, const Repeat &rep)
    {
        for (int i = 0; i < rep._n; i++) {
            os << rep._str;
            if (rep._delim == '\0' && i != rep._n - 1) {
                os << rep._delim;
            }
        }

        return os;
    }

    // ・split関数
    // 空文字列を渡すと要素数0のvectorを返す．
    std::vector <std::string> split(const std::string &str, const char delim=' ')
    {
        std::vector <std::string> result;
        std::string               word = "";
        for (char ch : str) {
            if (ch == delim) {
                if (word != "") {
                    result.push_back(word);
                }
                word = "";
            } else {
                word += ch;
            }
        }
        if (word != "") {
            result.push_back(word);
        }

        return std::move(result);
    }

    // デフォルト引数にすると上のオーバーロードとambiguousになってしまう．
    std::vector <std::string> split(const std::string &str, const std::string &delim)
    {
        std::vector <std::string> result;
        std::string               word         = "";
        size_t                    pos_on_delim = 0;
        std::string               delim_buf    = "";
        for (char ch : str) {
            if (ch != delim[pos_on_delim]) {
                // デリミタの現在位置と整合しない場合
                if (pos_on_delim != 0) {
                    // 途中まで整合していた場合
                    word        += delim_buf;
                    delim_buf    = "";
                    pos_on_delim = 0;
                }
                // 現在位置をヘッドにシークして再度判定する．
            }
            if (ch == delim[pos_on_delim]) {
                // デリミタと整合した場合
                delim_buf += ch;
                pos_on_delim++;
                if (pos_on_delim >= delim.size()) {
                    // デリミタの終端の場合wordをpushしてflushする．
                    if (word != "") {
                        result.push_back(word);
                    }
                    word         = "";
                    delim_buf    = "";
                    pos_on_delim = 0;
                }
            } else {
                // テリミタと整合しない場合
                word += ch;
            }
        }
        if (word != "") {
            result.push_back(word);
        }

        return std::move(result);
    }

    // ・trim関数
    // 文字列の前後のホワイトスペースを削る．
    // 削った結果なくなってしまう場合，空文字列を返す．
    // 中間にホワートスペースを含んでいたときも正しく動くように．
    std::string trim(const std::string &str)
    {
        size_t begin = 0, end = str.size();
        while (begin < end) {
            if (str[begin] != ' ' && str[begin] != '\t' && str[begin] != '\n' && str[begin] != '\r' && str[begin] != '\v') {
                break;
            }
            begin++;
        }
        while (begin < end) {
            if (str[end - 1] != ' ' && str[end - 1] != '\t' && str[end - 1] != '\n' && str[end - 1] != '\r' && str[end - 1] != '\v') {
                break;
            }
            end--;
        }

        return str.substr(begin, end - begin);
    }

    // ・配列つきenum
    // __VA_ARGS__は関数マクロの可変長引数を提供する．
    // 関数マクロ定義中の#演算子は変数名を文字列リテラルとしたものを表す．
    // つまり#演算子のついた変数は再評価されない．
    // インスタンスを作らないので，mapは必要なときに構築する．
    // classnameでラップしているのでスコープなしenumを用いる．
    // named_enum型の変数の宣言時にはTagが必要だが，具体的な整数定数を指すときはTagは不要．
#define named_enum(classname, ...)                                \
    class classname                                               \
    {                                                             \
    public:                                                       \
        enum Tag { __VA_ARGS__ };                                 \
    public:                                                       \
        static std::string to_string(int index)                   \
        {                                                         \
            static std::map <int, std::string> _name;             \
            if (_name.empty()) {                                  \
                auto elems = util::split(#__VA_ARGS__, ',');      \
                int  key   = 0;                                   \
                for (auto elem : elems) {                         \
                    auto tmp = util::split(elem, '=');            \
                    if (tmp.size() > 1) {                         \
                        key = std::stoi(tmp[1]);                  \
                    }                                             \
                    _name[key] = util::trim(std::string(tmp[0])); \
                    key++;                                        \
                }                                                 \
            }                                                     \
            return _name[index];                                  \
        }                                                         \
    };

    // ・imprementation_test関数
    // 処理系依存の環境をテストする関数．
    void imprementation_test()
    {
        std::cout << Repeat("-", 20) << std::endl;
        std::cout << "imprementation test" << std::endl;

        std::cout << std::endl;

        std::cout << "size test" << std::endl;
        std::cout << "sizeof(char) : " << sizeof(char) << "[byte]" << std::endl;
        std::cout << "sizeof(short): " << sizeof(short) << "[byte]" << std::endl;
        std::cout << "sizeof(int)  : " << sizeof(int) << "[byte]" << std::endl;
        std::cout << "sizeof(long) : " << sizeof(long) << "[byte]" << std::endl;

        std::cout << std::endl;

        std::cout << "downcast test" << std::endl;
        unsigned long ul = 549755813887;
        std::cout << "unsigned long : " << std::bitset <64>(static_cast <unsigned long>(ul)).to_string() << std::endl;
        std::cout << "unsigned int  : " << std::bitset <64>(static_cast <unsigned int>(ul)).to_string() << std::endl;
        std::cout << "unsigned short: " << std::bitset <64>(static_cast <unsigned short>(ul)).to_string() << std::endl;
        std::cout << "unsigned char : " << std::bitset <64>(static_cast <unsigned char>(ul)).to_string() << std::endl;

        std::cout << Repeat("-", 20) << std::endl;
    }

    // ・reverse関数
    // イテレータがない場合．
    template <class T>
    constexpr T reverse(const T &container)
    {
        T result = container;
        for (size_t i = 0; i < container.size(); i++) {
            result[i] = container[container.size() - 1 - i];
        }

        return std::move(result);
    }

    // ・slice関数
    // めっちゃ怪しいから使わない方がいいよ．
    template <int i, int j>
    struct Slice
    {
        template <template <int> class T, int n>
        constexpr static T <j - i> slice(const T <n> &container)
        {
            T <j - i> result;
            for (size_t k = i; k < j; k++) {
                result[k - i] = container[k];
            }

            return std::move(result);
        }
    };

    // ・HSVtoRGB関数
    // hue \in \set{Z}, saturation = 0, ..., 255, value = 0, ..., 255.
    boost::optional <std::tuple <int, int, int> > HSVtoRGB(int hue, int saturation, int value)
    {
        if (saturation < 0 || saturation > 255) {
            return boost::none;
        }
        if (value < 0 || value > 255) {
            return boost::none;
        }

        int    hi, p, q, t;
        double f;

        hi = (hue / 60) % 6;
        f  = hue / 60. - static_cast <int>(hue / 60.);
        p  = static_cast <int>(round(value * (1. - (saturation / 255.))));
        q  = static_cast <int>(round(value * (1. - (saturation / 255.) * f)));
        t  = static_cast <int>(round(value * (1. - (saturation / 255.) * (1. - f))));

        int red, green, blue;
        switch (hi)
        {
            case 0:
                red   = value;
                green = t;
                blue  = p;
                break;
            case 1:
                red   = q;
                green = value;
                blue  = p;
                break;
            case 2:
                red   = p;
                green = value;
                blue  = t;
                break;
            case 3:
                red   = p;
                green = q;
                blue  = value;
                break;
            case 4:
                red   = t;
                green = p;
                blue  = value;
                break;
            case 5:
                red   = value;
                green = p;
                blue  = q;
                break;
            default:

                return boost::none;
                break;
        }

        return std::tuple <int, int, int>(red, green, blue);
    }

    // ・color_encode関数
    std::string color_encode(const unsigned char red, const unsigned char green, const unsigned char blue)
    {
        std::ostringstream oss;
        oss << std::setfill('0');
        oss << "#";
        oss << std::setw(2) << std::hex << static_cast <unsigned int>(red);
        oss << std::setw(2) << std::hex << static_cast <unsigned int>(green);
        oss << std::setw(2) << std::hex << static_cast <unsigned int>(blue);

        return oss.str();
    }

    std::string color_encode(const std::array <unsigned char, 3> &rgb)
    {
        std::ostringstream oss;
        oss << std::setfill('0');
        oss << "#";
        oss << std::setw(2) << std::hex << static_cast <unsigned int>(rgb[0]);
        oss << std::setw(2) << std::hex << static_cast <unsigned int>(rgb[1]);
        oss << std::setw(2) << std::hex << static_cast <unsigned int>(rgb[2]);

        return oss.str();
    }

    // ・typename_of関数
    // typeid.name出力をデマングルする．
    // 野良C++erさんのコード．

    // __cxa_demangleがmallocして返すためメモリリークがある．
    char *demangle(const char *demangled)
    {
        int status;

        return abi::__cxa_demangle(demangled, 0, 0, &status);
    }

    // Tの型名を取得．
    // 参照型を区別しない．
    template <typename T>
    char *typename_of()
    {
        return demangle(typeid(T).name() );
    }

    // Tの型名を取得．
    // 参照型を区別しない．
    template <typename T>
    char *typename_of(T x)
    {
        return demangle(typeid(T).name() );
    }

    // Tの型名を取得．
    // 参照型を区別するが，boost::type<>の中に表示されるので冗長．
    template <typename T>
    char *typename_of_detail()
    {
        return demangle(typeid(boost::type <T> ).name() );
    }

    // Tの型名を取得．
    // 参照型を区別するが，boost::type<>の中に表示されるので冗長．
    template <typename T>
    char *typename_of_detail(T x)
    {
        return demangle(typeid(boost::type <T> ).name() );
    }
}

#endif
