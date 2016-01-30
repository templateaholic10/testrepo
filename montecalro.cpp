#include <iostream>
#include <fstream>
#include <exrandom>
#include <exmath>
#include <array>

constexpr int max_rep = 1000000;
constexpr int epsilons_num = 8;

template <typename T>
T PI_maker(std::ostream &os = std::cout, std::ostream &oslog = std::cout, std::ostream &oscount = std::cout)
{
    constexpr T edge = 1.;
    const T PI = std::PI<T>();

    std::array<T, epsilons_num> epsilons;
    epsilons[0] = 1e-0;
    epsilons[1] = 1e-1;
    epsilons[2] = 1e-2;
    epsilons[3] = 1e-3;
    epsilons[4] = 1e-4;
    epsilons[5] = 1e-5;
    epsilons[6] = 1e-6;
    epsilons[7] = 1e-7;
    std::array<bool, epsilons_num> flags;
    flags.fill(false);

    std::Uniform<T> uniform(0., edge);

    int count = 0;
    int incount = 0;
    T my_PI;
    for (int i = 0; i < max_rep; i++) {
        const T x = uniform();
        const T y = uniform();
        const T r = std::sqrt(x*x + y*y);

        count++;
        if (r < edge) {
            incount++;
        }
        my_PI = 4. * incount / count;
        os << my_PI << std::endl;
        oslog << std::log(my_PI) << std::endl;
        for (int j = 0; j < epsilons_num; j++) {
            if (!flags[j] && std::abs(my_PI - PI) < epsilons[j]) {
                oscount << count << std::endl;
                flags[j] = true;
            }
        }
        if (flags.back()) {
            break;
        }
    }
    std::cout << count << std::endl;
    return my_PI;
}

int main()
{
    std::ofstream fout("hoge.csv");
    std::ofstream fout2("piyo.csv");
    std::ofstream fout3("fuga.csv");
    PI_maker<double>(fout, fout2, fout3);
    fout3.close();
    fout2.close();
    fout.close();
    return 0;
}
