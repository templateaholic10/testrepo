//  動機：次元チェックをC++に担保させたい．
//  具体的には，次元を型システムによって管理し，加算を同じ型どうしについてのみ定義する．
//  このとき，SI基本次元の順序対である型を定義したい．

#include <iostream>
#include <functional>
#include <algorithm>
#include <array>
#include <vector>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include "../util.hpp"

// stlの実行時アルゴリズムtransformの例．
namespace trans {
    template <typename T>
    struct Power
    {
        T operator()(T obj)
        {
            return obj * obj;
        }
    };

    void test()
    {
        std::vector <double> v = { 1., 2., 3. };

        std::cout << "test std::transform :" << std::endl;
        std::cout << "v : ";
        std::for_each(v.begin(), v.end(), [](double d) {
            std::cout << d << " ";
        });
        std::cout << std::endl;
        std::cout << "std::transform(v.begin(),v.end(),v.begin(),Power<double>())" << std::endl;
        std::transform(v.begin(), v.end(), v.begin(), Power <double>());
        std::cout << "v : ";
        std::for_each(v.begin(), v.end(), [](double d) {
            std::cout << d << " ";
        });
        std::cout << std::endl;
    }
}

namespace dimension {
    namespace mpl = boost::mpl;
    using namespace mpl::placeholders;
    // 基本次元は質量，長さ，時間，電流，温度，光度，物質量の順とする．
    // 型の列mpl::vectorの整数定数ラッパがmpl::vector_c．
    // static_assert(mpl::equal<mpl::vector_c<int,0,1,0>, mpl::vector<mpl::int_<0>,mpl::int_<1>,mpl::int_<0>>>::value, "vector_c error!");

    using Scalar = mpl::vector7_c <int, 0, 0, 0, 0, 0, 0, 0>;

    using Mass                = mpl::vector7_c <int, 1, 0, 0, 0, 0, 0, 0>;
    using Length              = mpl::vector7_c <int, 0, 1, 0, 0, 0, 0, 0>;
    using Time                = mpl::vector7_c <int, 0, 0, 1, 0, 0, 0, 0>;
    using Charge              = mpl::vector7_c <int, 0, 0, 0, 1, 0, 0, 0>;
    using Temperature         = mpl::vector7_c <int, 0, 0, 0, 0, 1, 0, 0>;
    using Intensity           = mpl::vector7_c <int, 0, 0, 0, 0, 0, 1, 0>;
    using Amount_of_substance = mpl::vector7_c <int, 0, 0, 0, 0, 0, 0, 1>;

    using Velocity     = mpl::vector7_c <int, 0, 1, -1, 0, 0, 0, 0>;
    using Acceleration = mpl::vector7_c <int, 0, 1, -2, 0, 0, 0, 0>;
    using Momentum     = mpl::vector7_c <int, 1, 1, -1, 0, 0, 0, 0>;
    using Force        = mpl::vector7_c <int, 1, 1, -2, 0, 0, 0, 0>;

    template <class T, class Dimensions>
    struct quantity
    {
        using type       = quantity <T, Dimensions>;
        using value_type = T;
        using dim        = Dimensions;
    public:
        explicit quantity(T x)
            : m_value(x)
        {
        }

        template <class OtherDimensions>
        quantity(quantity <T, OtherDimensions> const &rhs)
            : m_value(rhs.value())
        {
            static_assert(mpl::equal <Dimensions, OtherDimensions>::type::value, "error!(implicit cast to unequal dimension)");
        }

        T value() const
        {
            return m_value;
        }

    private:
        T m_value;
    };

    // 整数定数ラッパーを表示する関数
    struct print_int {
        template <class T>
        void operator()(std::ostream& os, T x)
        {
            os << T::value;
        }
    };

    template <class T, class D>
    std::ostream&operator<<(std::ostream &os, quantity <T, D> x)
    {
        os << "<";
        // mpl::for_eachによって関数に渡されるのは値初期化されたオブジェクト．
        mpl::for_each <D>(std::bind(print_int(), std::ref(std::cout), std::placeholders::_1));
        os << ">";
        os << x.value();

        return os;
    }

    template <class T, class D>
    quantity <T, D> operator+(quantity <T, D> x, quantity <T, D> y)
    {
        return quantity <T, D>(x.value() + y.value());
    }

    template <class T, class D>
    quantity <T, D> operator-(quantity <T, D> x, quantity <T, D> y)
    {
        return quantity <T, D>(x.value() - y.value());
    }

    // mpl::transformメタ関数は
    // mpl::transform<class Seq1, class Seq2, class BinaryOperation>
    // の形で用いる．
    // 型シーケンスSeq1とSeq2の要素を走査してBinaryOperationを施し，結果の型シーケンスをtypeで返す．
    // このとき，当然BinaryOperationにはメタ関数を指定したいが，メタ関数はクラステンプレートであって型ではないのでメタ関数の引数にできない．

    // ＜方法1＞メタ関数をメタ関数クラスでラップし，メタ関数クラスを指定する．
    struct plus_f
    {
        // 古典的にはこうする．
        // template <class T1, class T2>
        // struct apply
        // {
        //     using type = typename mpl::plus <T1, T2>::type;
        // };

        // メタ関数転送（メタ関数を継承して基底クラステンプレートのtypeを参照する）を使ってもよい
        template <class T1, class T2>
        struct apply : mpl::plus <T1, T2>
        {
        };
    };

    template <class T, class D1, class D2>
    quantity <T, typename mpl::transform <D1, D2, plus_f>::type> operator*(quantity <T, D1> x, quantity <T, D2> y)
    {
        using dim = typename mpl::transform <D1, D2, plus_f>::type;

        return quantity <T, dim>(x.value() * y.value());
    }

    // ＜方法2＞プレースホルダによってメタ関数をクラス化する．
    template <class T, class D1, class D2>
    quantity <T, typename mpl::transform <D1, D2, mpl::minus <_1, _2> >::type> operator/(quantity <T, D1> x, quantity <T, D2> y)
    {
        using dim = typename mpl::transform <D1, D2, mpl::minus <_1, _2> >::type;

        return quantity <T, dim>(x.value() / y.value());
    }

    void test()
    {
        std::cout << "test dimension analysis :" << std::endl;
        quantity <double, Mass> m(5.0);
        std::cout << m << std::endl;
        quantity <double, Acceleration> a(9.8);
        std::cout << a << std::endl;

        quantity <double, Force> f = m * a;
        std::cout << f << std::endl;
    }
}
