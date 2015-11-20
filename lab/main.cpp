#include <iostream>
#include <util>
#include <util_eigen>
#include "eigen_io.hpp"
#include "exfstream.hpp"

int main(int argc, char const *argv[])
{
    constexpr int p = 5;
    Eigen::dVector<Eigen::Dynamic> v = Eigen::rand_uniform<double, 5, 1>(0., 1.);
    _TYPE(v)
    _DISPLAY(v.rows())
    _DISPLAY(v.cols())
    const std::string filename("hoge.txt");
    dump(filename, v);
    load(filename, v);

    return 0;
}
