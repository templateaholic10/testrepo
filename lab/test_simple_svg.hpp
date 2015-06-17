#ifndef TEST_SIMPLE_SVG
#define TEST_SIMPLE_SVG

#include <simple_svg_1.0.0.hpp>

namespace test_simple_svg {
    using namespace svg;
    // Tの3頂点を与えると，内部に再帰的にpinwheel_tilingを生成する．

    class Pinwheel
    {
    public:
        Pinwheel()  = delete;
        ~Pinwheel() = delete;
    private:
        static constexpr double min_tile = 3.;

        // （内部的）タイリングメソッド
        static void _tile(Document &doc, Point p1, Point p2, Point p3, bool face, const Stroke &stroke)
        {
            // 角度を求める．
            double theta;
            // 色を求める．
            int r, g, b;
            // 三角形を描画する．
            doc << (Polygon(Color(r, g, b), stroke) << Point(20, 70) << Point(25, 72) << Point(33, 70));
            // 短辺を求めて，小さければ再帰しない．
            double d2 = p1
        }

    public:
        // タイリングメソッド
        static void tile(Document &doc, Point p1, Point p2, Point p3)
        {
        }
    };
}

#endif
