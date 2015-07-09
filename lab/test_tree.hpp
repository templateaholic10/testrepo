#ifndef TEST_TREE
#define TEST_TREE

#include <fstream>
#include "typetree.hpp"
#include "util.hpp"
#include "dot_outer.hpp"
#include "metadata.hpp"

namespace tree {
    namespace typetree {
        void test_typetree()
        {
            std::cout << util::Repeat("-", 20) << std::endl;
            std::cout << "Typetree test" << std::endl;

            std::cout << "ex1. オブジェクト型の従属関係を定義する" << std::endl;
            constexpr auto str1 = animal::p_seq;
            std::cout << str1.c_str() << std::endl;
            // Paren型．
            using _Paren1 = tree::shape::paren::Paren <util::paren_to_bitseq(sprout::fixed::reverse(str1)).to_ulong(), str1.size()>;
            // Tree型．
            using _TreeShape1 = tree::shape::TreeShape <_Paren1>;
            // メタデータ木．
            using _Meta_Tree1 = Tree <_TreeShape1, animal::Elements>;

            std::cout << "[adjucency list expression]" << std::endl;
            std::cout << Tree_Out <_Meta_Tree1>() << std::endl;

            std::ofstream fout("typetree1.dot");
            fout << (dot::Dot("typetree1") << Tree_Out <_Meta_Tree1>()) << std::endl;
            fout.close();

            std::cout << "FIND(animal::Akari): " << FIND <_Meta_Tree1, animal::Akari>::value << std::endl;
            std::cout << "FIND(int): " << FIND <_Meta_Tree1, int>::value << std::endl;
            std::cout << "DIRECTLINE(animal::Animal, animal::Man): " << DIRECTLINE <_Meta_Tree1, animal::Animal, animal::Man>::value << std::endl;
            std::cout << "DIRECTLINE(animal::Human, animal::Otaku): " << DIRECTLINE <_Meta_Tree1, animal::Human, animal::Otaku>::value << std::endl;

            std::cout << std::endl;

            std::cout << "ex2. 派生型の関係を参照する" << std::endl;
            constexpr auto str2 = derivation::p_seq;
            std::cout << str2.c_str() << std::endl;
            // Paren型．
            using _Paren2 = tree::shape::paren::Paren <util::paren_to_bitseq(sprout::fixed::reverse(str2)).to_ulong(), str2.size()>;
            // Tree型．
            using _TreeShape2 = tree::shape::TreeShape <_Paren2>;
            // メタデータ木．
            using _Meta_Tree2 = Tree <_TreeShape2, derivation::Elements>;

            std::cout << "[adjucency list expression]" << std::endl;
            std::cout << Tree_Out <_Meta_Tree2>(" -> ", ", ", true) << std::endl;

            fout.open("typetree2.dot");
            fout << (dot::Dot("typetree2") << Tree_Out <_Meta_Tree2>(" -> ", ", ", true)) << std::endl;
            fout.close();

            std::cout << "FIND(int * const): " << FIND <_Meta_Tree2, int *const>::value << std::endl;
            std::cout << "FIND(const int * const): " << FIND <_Meta_Tree2, const int *const>::value << std::endl;
            std::cout << "DIRECTLINE(int *, int * const *): " << DIRECTLINE <_Meta_Tree2, int *, int *const *>::value << std::endl;
            std::cout << "DIRECTLINE(int *, int (*)[]): " << DIRECTLINE <_Meta_Tree2, int *, int(*)[]>::value << std::endl;

            std::cout << util::Repeat("-", 20) << std::endl;
        }
    }
}

#endif
