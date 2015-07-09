char *gets(char *str);

#include "functor.hpp"
#include "utiltest.hpp"
#include "test_simple_svg.hpp"
// #include "unionbitarray.hpp"
#include "test_sprout.hpp"
// #include "typetree.hpp"
#include "dot_outer.hpp"

named_enum(RGB, R, G, B);

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
    // tree::typetree::test_typetree();
    // using RGB = std::array<unsigned char, 3>;
    // RGB rgb = RGB({255, 0, 0});
    // std::cout << util::color_encode(rgb) << std::endl;

    // dot::Dot hoge = dot::Dot();
    // hoge.node_master.shape = dot::Node::Shape::box;
    // hoge.node_master.style = dot::Node::Style::dotted;
    // hoge.node_master.color = dot::RGB({255,0,0});
    // hoge.edges.push_back(dot::Edge(1,2));
    // hoge.edges.push_back(dot::Edge(2,3));
    // hoge.edges.push_back(dot::Edge(3,1));
    // std::cout << hoge << std::endl;

    auto splited = util::split("sena->ak-ari-->ichigo", "->");
    std::for_each(splited.begin(), splited.end(), [](const std::string& str){std::cout << str << std::endl;});

    return 0;
}
