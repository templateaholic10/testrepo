char *gets(char *str);

#include "EM_algorithm.hpp"
// #include "data_struct.hpp"
// #include "pdist.hpp"
// #include "statistic_utiltest.hpp"

int main(int argc, char const *argv[])
{
    // EM::test_EM_gaussian_mixtures();
    // const auto numberings = util::make_array<std::string>("01", "02", "03", "04", "05", "06");
    const auto numberings = util::make_array<std::string>("01");
    for (auto numbering : numberings) {
        EM::test_EM_initial_value(numbering);
    }
    // statistic::Probability_distribution <2, statistic::GAUSSIAN_MIXTURES <2> >::test();
    // statistic_util::testdescreteDV1();
    // statistic_util::testdescreteDV2();

    return 0;
}
