char *gets(char *str);

#include "functor.hpp"
#include "utiltest.hpp"
#include "test_simple_svg.hpp"
#include "unionbitarray.hpp"
#include "test_sprout.hpp"
#include "compile_tree.hpp"

int main(int argc, char const *argv[])
{
    // // functor::test();
    //
    // using namespace util;
    //
    // // 1次元arrayのテスト
    // constexpr int  family_num        = 4;
    // constexpr auto family_birthmonth = make_array <int>(12, 9, 11, 9);
    // static_assert(array_size(family_birthmonth) == family_num, "family_birthmonth size error!");
    //
    // // 多次元arrayのテスト
    // constexpr multi_array <int, 3, 3, 3> cubic = {         {
    //                                                            { { { { 0,  1,   2   } }, { { 3,  4,  5   } }, { { 6,  7,  8   } } } },
    //                                                            { { { { 9,  10,  11  } }, { { 12, 13, 14  } }, { { 15, 16, 17  } } } },
    //                                                            { { { { 18, 19,  20  } }, { { 21, 22, 23  } }, { { 24, 25, 26  } } } }
    //                                                        } };
    // constexpr auto                       small_cube = make_common_array(make_common_array(0.0, 3.6, 2.6), make_common_array(9.5, 3.8, 9.3));
    //
    // // nresult_of関数のテスト
    // testnresult_of();
    //
    // // apply関数のテスト
    // // testapply();
    //
    // // convert_arrayのテスト
    // // testconvert_array();

    // test_simple_svg::test_pinwheel();

    // unionbitarray::testUnionbitarray();
    // constexpr auto hoge = test_sprout::test_sprout<1000>();
    // std::cout << hoge << std::endl;
    // constexpr auto str = sprout::to_string("((())()())");
    // constexpr auto bitseq = util::paren_to_bitseq(str);
    // std::cout << bitseq.to_string() << std::endl;
    paren::test_paren();
    tree::shape::test_graph();

    return 0;
}
