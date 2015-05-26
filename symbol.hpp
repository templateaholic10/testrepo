#include <iostream>
#include <vector>
#include <string>

namespace functree {
    class Symbol;

    class Symbol {
    public:
        Symbol() = delete;  // コンストラクタ
        Symbol(const std::string& name);  // コンストラクタ
        ~Symbol() = default;  // デストラクタ
        Symbol(Symbol const&);  // コピーコンストラクタ
        Symbol& operator =(Symbol const&);  // コピー代入演算子
        Symbol(Symbol&&);  // ムーブコンストラクタ
        Symbol& operator =(Symbol&&);  // ムーブ代入演算子

        const std::string str() const;
    private:
        std::string _name;

        static std::vector<std::string> _symbolTable;
    };
}
