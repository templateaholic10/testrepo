char *gets(char *str);

#include <iostream>

// Eigenのコア機能とジオメトリ機能を使う
#include <Eigen/Core>
#include <Eigen/Geometry>


int main()
{
    // 以下サンプルコードでは名前空間Eigenを省略
    using namespace Eigen;

    // 二次元ベクトルv1の定義。初期値は未定義
    Vector2f v1;

    // 初期値はコンストラクタで指定できる
    Vector2f v2(1.0f, 0.5f);
    Vector3f v3(0.0f, 1.0f, -1.0f);

    // 内容を表示
    std::cout << "v2\n" << v2 << std::endl;
    std::cout << "v3\n" << v3 << std::endl;

    // double型3x3行列の定義。初期値は未定義
    Matrix3d m;

    // 行列の初期値はコンストラクタで指定できないが
    // 値は以下のようにまとめて代入できる
    m << 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0;

    std::cout << "m\n" << m << std::endl;
}
