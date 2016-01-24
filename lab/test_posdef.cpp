#include <iostream>
#include <exeigen>
#include <randeigen>
#include <debug>
#include <type_traits>

int main()
{
    // constexpr int N = 1;
    // constexpr int p = 5;
    // using T = double;
    // using v_type = Eigen::Vector<T, p>;
    // using M_type = Eigen::Matrix<T, p, p>;
    // std::Wishart<M_type> wishart(M_type::Identity(), p);
    //
    // for (size_t i = 0; i < N; i++) {
    //     const M_type M = wishart();
    //     if (Eigen::positive_definite(M)) {
    //         _PRINT(M)
    //     }
    // }
    using T = typename std::common_type<int, double>::type;
    _TYPE(T)
    // constexpr double d = std::fmod(1.7, 1.);

    return 0;
}
