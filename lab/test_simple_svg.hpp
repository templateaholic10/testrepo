#ifndef TEST_SIMPLE_SVG
#define TEST_SIMPLE_SVG

#include <math.h>
#include <array>
#include <algorithm>
#include <simple_svg_1.0.0.hpp>
#include "util.hpp"

namespace test_simple_svg {
    using namespace svg;
    // Tの3頂点を与えると，内部に再帰的にpinwheel_tilingを生成する．

    inline double length(const Point &P1, const Point &P2)
    {
        return sqrt((P2.x - P1.x) * (P2.x - P1.x) + (P2.y - P1.y) * (P2.y - P1.y));
    }

    inline Point inner_divide(const Point &P1, const Point &P2, const double t)
    {
        return Point((1 - t) * (P1.x) + t * (P2.x), (1 - t) * (P1.y) + t * (P2.y));
    }

    class Pinwheel
    {
        using RGB = std::array <int, 3>;
    public:
        Pinwheel()  = delete;
        ~Pinwheel() = delete;
    private:
        static constexpr double    min_tile = 30.;
        static constexpr double    epsilon  = 10e-6;
        static Stroke              stroke;
        static std::array <RGB, 2> face_color;
        static std::array <RGB, 2> tail_color;

        // （内部的）タイリングメソッド
        static void _tile(Document &doc, Point &P1, Point &P2, Point &P3, bool face)
        {
            // 角度を求める．
            double theta = atan2(P2.y - P1.y, P2.x - P1.x);
            // 色を求める．
            RGB    rgb;
            double t         = theta / (2 * M_PI) + 1 / 2;
            auto   get_color = [ = ](int from, int to) -> int {
                                   return static_cast <int>((1 - t) * from + t * to);
                               };

            if (face) {
                std::transform(face_color[0].begin(), face_color[0].end(), face_color[1].begin(), rgb.begin(), get_color);
            } else {
                std::transform(tail_color[0].begin(), tail_color[0].end(), tail_color[1].begin(), rgb.begin(), get_color);
            }
            // 三角形を描画する．
            doc << (Polygon(Color(rgb[0], rgb[1], rgb[2]), stroke) << P1 << P2 << P3);
            // 短辺を求めて，小さければ再帰しない．
            double d = length(P1, P2);
            if (d < min_tile) {
                return;
            }
            // 再帰．
            Point Q = inner_divide(P3, P2, 0.2);
            Point R = inner_divide(P1, P2, 0.5);
            Point S = inner_divide(P1, Q, 0.5);
            Point T = inner_divide(P2, Q, 0.5);
            _tile(doc, Q, P1, P3, !face);
            _tile(doc, S, R, P1, !face);
            _tile(doc, T, P2, R, !face);
            _tile(doc, S, R, Q, face);
            _tile(doc, T, Q, R, face);
        }

    public:
        // タイリングメソッド
        static void tile(Document &doc, const Point &P1, const Point &P2, const Point &P3)
        {
            struct Point_for_sort
            {
                Point_for_sort(const Point &_P, double d)
                    : P(_P), opposite_side(d)
                {
                }

                Point  P;
                double opposite_side;
            };

            auto points = util::make_array <Point_for_sort>(Point_for_sort(P1, length(P2, P3)), Point_for_sort(P2, length(P3, P1)), Point_for_sort(P3, length(P1, P2)));
            std::sort(points.begin(), points.end(), [](const Point_for_sort &Ps1, const Point_for_sort &Ps2) {
                return Ps1.opposite_side < Ps2.opposite_side;
            });

            if ((fabs(points[1].opposite_side / points[0].opposite_side - 2.) > epsilon) || (fabs(points[2].opposite_side / points[0].opposite_side - sqrt(5)) > epsilon)) {
                std::cerr << "initial triangle is not similar to T." << std::endl;

                return;
            }

            _tile(doc, points[2].P, points[0].P, points[1].P, true);
        }

        static void set_color(int r1, int g1, int b1, int r2, int g2, int b2, bool face)
        {
            if (r1 < 0 || r1 > 255) {
                return;
            }
            if (g1 < 0 || g1 > 255) {
                return;
            }
            if (b1 < 0 || b1 > 255) {
                return;
            }
            if (r2 < 0 || r2 > 255) {
                return;
            }
            if (g2 < 0 || g2 > 255) {
                return;
            }
            if (b2 < 0 || b2 > 255) {
                return;
            }
            if (face) {
                face_color = { r1, g1, b1, r2, g2, b2 };
            } else {
                tail_color = { r1, g1, b1, r2, g2, b2 };
            }
        }
    };

    Stroke                        Pinwheel::stroke     = Stroke(0., Color(0, 0, 0));
    std::array <Pinwheel::RGB, 2> Pinwheel::face_color = { 255, 255, 255, 211, 84, 0 };
    std::array <Pinwheel::RGB, 2> Pinwheel::tail_color = { 255, 255, 255, 142, 68, 173 };

    void test_pinwheel()
    {
        // 色の設定
        // int r1, g1, b1, r2, g2, b2;
        // if (auto const rgb = util::HSVtoRGB(0, 0, 0)) {
        //     std::tie(r1, g1, b1) = *rgb;
        // } else {
        //     return;
        // }
        // if (auto const rgb = util::HSVtoRGB(0, 0, 0)) {
        //     std::tie(r2, g2, b2) = *rgb;
        // } else {
        //     return;
        // }
        // Pinwheel::set_color(r1, g1, b1, r2, g2, b2, true);
        // if (auto const rgb = util::HSVtoRGB(0, 0, 0)) {
        //     std::tie(r1, g1, b1) = *rgb;
        // } else {
        //     return;
        // }
        // if (auto const rgb = util::HSVtoRGB(0, 0, 0)) {
        //     std::tie(r2, g2, b2) = *rgb;
        // } else {
        //     return;
        // }
        // Pinwheel::set_color(r1, g1, b1, r2, g2, b2, false);

        Pinwheel::set_color(255, 129, 25, 234, 245, 67, true);
        Pinwheel::set_color(158, 40, 163, 213, 132, 224, false);

        int        width = 1200, height = 600;
        Dimensions dimensions(width, height);
        Document   doc("test.svg", Layout(dimensions, Layout::BottomLeft));

        Pinwheel::tile(doc, Point(0, 0), Point(width, 0), Point(0, height));
        doc.save();
    }
}

#endif
