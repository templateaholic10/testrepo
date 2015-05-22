char *gets(char *str);

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <boost/optional.hpp>

namespace symbfunc {
    // シンボリッククラス
    struct Symbol {
        std::string name;
    };

    // 基底クラス
    template <class T>
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
        virtual boost::optional<T> eval(boost::optional<T> t) const = 0;

        // 微分
        virtual Basefunc <T> d() const = 0;

    private:
        std::vector<std::unique_ptr<Basefunc<T>>> _operand;
    };
}
