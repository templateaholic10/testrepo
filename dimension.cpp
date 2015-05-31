//  動機：次元チェックをC++に担保させたい．
//  具体的には，次元を型システムによって管理し，加算を同じ型どうしについてのみ定義する．
//  このとき，SI基本次元の順序対である型を定義したい．

char *gets(char *str);

#include <iostream>
#include <boost/mpl/vector_c.hpp>

namespace mpl = boost::mpl;
// 基本次元は質量，長さ，時間，電流，温度，光度，物質量の順とする．
using Scalar                = mpl::vector_c<int,0,0,0,0,0,0,0>;

using Mass                  = mpl::vector_c<int,1,0,0,0,0,0,0>;
using Length                = mpl::vector_c<int,0,1,0,0,0,0,0>;
using Time                  = mpl::vector_c<int,0,0,1,0,0,0,0>;
using Charge                = mpl::vector_c<int,0,0,0,1,0,0,0>;
using Temperature           = mpl::vector_c<int,0,0,0,0,1,0,0>;
using Intensity             = mpl::vector_c<int,0,0,0,0,0,1,0>;
using Amount_of_substance   = mpl::vector_c<int,0,0,0,0,0,0,1>;

using Velocity              = mpl::vector_c<int,0,1,-1,0,0,0,0>;
using Acceleration          = mpl::vector_c<int,0,1,-2,0,0,0,0>;
using Momentum              = mpl::vector_c<int,1,1,-1,0,0,0,0>;
using Force                 = mpl::vector_c<int,1,1,-2,0,0,0,0>;

template <class T, class Dimensions>
struct quantity
{
public:
    explicit quantity(T x)
    : m_value(x)
    {};
    T value() const {return m_value;}
private:
    T m_value;
};
