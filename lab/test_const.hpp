#ifndef TEST_CONST
#define TEST_CONST

#include <iostream>

namespace test_const {

    void test_const()
    {
        // 型指定子は前方結合性．
        // (*)などのようにくくると前後が先に結合する．
        // const TはT constと同じ．

        char ch = 'a';
        char * chp = &ch;
        char ** chpp = &chp;
        const char cch = 'b';

        // 隣接したconstとTは交換可能．
        // constが何番目のアスタリスクの間に入るかによる．
        // つまり先頭のconstはTと交換して前から読めばよい．
        // constの基本は前方結合．

        // 1段ポインタ
        const char * cchp;
        char const * chcp;
        char * const chpc = &ch;

        char const * const chcpc = &cch;

        static_assert(std::is_same<decltype(cchp), decltype(chcp)>::value, "error");

        // 2段ポインタ
        const char ** cchpp;
        char const ** chcpp;
        char * const * chpcp;
        char ** const chppc = &chp;

        char const * const * const chcpcpc = &chcpc;

        static_assert(std::is_same<decltype(cchpp), decltype(chcpp)>::value, "error");

        // 3段ポインタ
        const char *** cchppp;
        char const *** chcppp;
        char * const ** chpcpp;
        char ** const * chppcp;
        char *** const chpppc = &chpp;

        static_assert(std::is_same<decltype(cchppp), decltype(chcppp)>::value, "error");

        const int a[] = {0, 1, 2};
        const int b[] = {3, 4, 5, 6};
        int t1 = 1, t2 = 2, t3 = 3;
        const int ct1 = 1, ct2 = 2, ct3 = 3;

        // const int (*)[]型
        // (*)なのでその前後が結合する．
        // つまりconst int[]へのポインタ．
        const int (*p_ciarray)[3] = &a;

        // const int (*const)[]型
        // (*const)なのでその前後が結合する．
        // つまりconst int[]へのconstポインタ．
        const int (*const cp_ciarray)[3] = &a;

        // const int *[]型
        // 基本は前方結合なのでconst int *型の配列．
        const int *array_cip[] = {&ct1, &ct2, &ct3};

        // int const *[]型
        // int const *型の配列．
        const int *array_icp[] = {&ct1, &ct2, &ct3};

        // int * const[]型
        // int * const型の配列．
        int * const array_ipc[] = {&t1, &t2, &t3};
    }

}

#endif
