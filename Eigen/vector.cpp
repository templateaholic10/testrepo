char* gets(char* str);

#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

int main()
{
    using namespace Eigen;

    Vector3f v;
    v = Vector3f::Identity();

    v = Vector3f::Zero();

    v = Vector3f::Ones();

    v = Vector3f::UnitX();

    v = Vector3f::UnitY();

    v = Vector3f::UnitZ();

    v = Vector3f::Random();

    v = Vector3f::Constant(0.5f);

    v = Vector3f::UnitX();

    v << 0.5f, 1.2f, -2.0f;

    Vector3f v1 = v;

    Vector3f v2 = v + v1;

    float dot = v.dot(v1);

    return 0;
}
