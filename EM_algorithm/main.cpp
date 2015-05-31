char *gets(char *str);

// #include "pdist.hpp"
#include "statistic_utiltest.hpp"

int main(int argc, char const *argv[])
{
    // using namespace statistic;

    // test<GAUSSIAN>();
    statistic_util::testdescreteDV1();
    statistic_util::testdescreteDV2();

    return 0;
}
