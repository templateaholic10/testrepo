/*! @file
    @brief functionヘッダの拡張
    @auther yanteyon10
    @date 11/13
*/

#ifndef EXFUNCTION_HPP
#define EXFUNCTION_HPP

#include <functional>

namespace std {
    /*! @namespace argument
        @brief 引数型を定義．使用は未定
    */
    namespace argument {
        struct _1 {
            using type = _1;
            static constexpr int value = 1;
        };

        struct _2 {
            using type = _2;
            static constexpr int value = 2;
        };

        struct _3 {
            using type = _3;
            static constexpr int value = 3;
        };
    }

    /*! @struct Composite
        @brief fとgの合成関数f \cdot gをつくる関数のガワ．本体はstaticメンバ関数eval
        @tparam place gの返り値のf内での位置
    */
    template <int place = 0>
    struct Composite;

    template <>
    struct Composite <0> {
        /*! @brief [1] fの引数が1つ，gの引数がない場合
        */
        /*! @brief [1-1] Gret: 値
        */
        template <class Fret, class Gret>
        static std::function <Fret()> eval(const std::function <Fret(Gret)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret()>;

            return result_type([&]() -> Fret {
                return f(g());
            });
        }

        /*! @brief [1-2] Gret: 参照
        */
        template <class Fret, class Gret>
        static std::function <Fret()> eval(const std::function <Fret(const Gret&)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret()>;

            return result_type([&]() -> Fret {
                return f(g());
            });
        }

        /*! @brief [2] fの引数が1つ，gの引数がある場合
        */
        /*! @brief [2-1] Gret: 値，Ghead: 値，Gargs: 値
        */
        template <class Fret, class Gret, class Ghead, class ... Gargs>
        static std::function <Fret(Ghead, Gargs ...)> eval(const std::function <Fret(Gret)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(Ghead, Gargs ...)>;

            return result_type([&](Ghead ghead, Gargs ... gargs) -> Fret {
                return f(g(ghead, gargs ...));
            });
        }

        // /*! @brief [2-2] Gret: 値，Ghead: 値，Gargs: 参照
        // */
        // template <class Fret, class Gret, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead, Gargs& ...)> eval(const std::function <Fret(Gret)> &f, const std::function <Gret(Ghead, Gargs& ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead, Gargs& ...)>;
        //
        //     return result_type([&](Ghead ghead, Gargs& ... gargs) -> Fret {
        //         return f(g(ghead, gargs ...));
        //     });
        // }
        //
        // /*! @brief [2-3] Gret: 値，Ghead: 参照，Gargs: 値
        // */
        // template <class Fret, class Gret, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead&, Gargs ...)> eval(const std::function <Fret(Gret)> &f, const std::function <Gret(Ghead&, Gargs ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead&, Gargs ...)>;
        //
        //     return result_type([&](Ghead& ghead, Gargs ... gargs) -> Fret {
        //         return f(g(ghead, gargs ...));
        //     });
        // }

        /*! @brief [2-4] Gret: 参照，Ghead: 値，Gargs: 値
        */
        template <class Fret, class Gret, class Ghead, class ... Gargs>
        static std::function <Fret(Ghead, Gargs ...)> eval(const std::function <Fret(const Gret&)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(Ghead, Gargs ...)>;

            return result_type([&](Ghead ghead, Gargs ... gargs) -> Fret {
                return f(g(ghead, gargs ...));
            });
        }

        // /*! @brief [2-5] Gret: 値，Ghead: 参照，Gargs: 参照
        // */
        // template <class Fret, class Gret, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead&, Gargs& ...)> eval(const std::function <Fret(Gret)> &f, const std::function <Gret(Ghead&, Gargs& ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead&, Gargs& ...)>;
        //
        //     return result_type([&](Ghead& ghead, Gargs& ... gargs) -> Fret {
        //         return f(g(ghead, gargs ...));
        //     });
        // }
        //
        // /*! @brief [2-6] Gret: 参照，Ghead: 参照，Gargs: 値
        // */
        // template <class Fret, class Gret, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead&, Gargs ...)> eval(const std::function <Fret(Gret&)> &f, const std::function <Gret(Ghead&, Gargs ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead&, Gargs ...)>;
        //
        //     return result_type([&](Ghead& ghead, Gargs ... gargs) -> Fret {
        //         return f(g(ghead, gargs ...));
        //     });
        // }
        //
        // /*! @brief [2-7] Gret: 参照，Ghead: 値，Gargs: 参照
        // */
        // template <class Fret, class Gret, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead, Gargs& ...)> eval(const std::function <Fret(Gret&)> &f, const std::function <Gret(Ghead, Gargs& ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead, Gargs& ...)>;
        //
        //     return result_type([&](Ghead ghead, Gargs& ... gargs) -> Fret {
        //         return f(g(ghead, gargs ...));
        //     });
        // }
        //
        // /*! @brief [2-8] Gret: 参照，Ghead: 参照，Gargs: 参照
        // */
        // template <class Fret, class Gret, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead&, Gargs& ...)> eval(const std::function <Fret(Gret&)> &f, const std::function <Gret(Ghead&, Gargs& ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead&, Gargs& ...)>;
        //
        //     return result_type([&](Ghead& ghead, Gargs& ... gargs) -> Fret {
        //         return f(g(ghead, gargs ...));
        //     });
        // }

        /*! @brief [3] fの引数が複数，gの引数がない場合
        */
        /*! @brief [3-1] Gret: 値，Fhead: 値，Fargs: 値
        */
        template <class Fret, class Gret, class Fhead, class ... Fargs>
        static std::function <Fret(Fhead, Fargs ...)> eval(const std::function <Fret(Gret, Fhead, Fargs ...)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret(Fhead, Fargs ...)>;

            return result_type([&](Fhead fhead, Fargs ... fargs) -> Fret {
                return f(g(), fhead, fargs ...);
            });
        }

        // /*! @brief [3-2] Gret: 値，Fhead: 値，Fargs: 参照
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs>
        // static std::function <Fret(Fhead, Fargs& ...)> eval(const std::function <Fret(Gret, Fhead, Fargs& ...)> &f, const std::function <Gret()> &g)
        // {
        //     using result_type = std::function <Fret(Fhead, Fargs& ...)>;
        //
        //     return result_type([&](Fhead fhead, Fargs& ... fargs) -> Fret {
        //         return f(g(), fhead, fargs ...);
        //     });
        // }

        // /*! @brief [3-3] Gret: 値，Fhead: 参照，Fargs: 値
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs>
        // static std::function <Fret(Fhead&, Fargs ...)> eval(const std::function <Fret(Gret, Fhead&, Fargs ...)> &f, const std::function <Gret()> &g)
        // {
        //     using result_type = std::function <Fret(Fhead&, Fargs ...)>;
        //
        //     return result_type([&](Fhead& fhead, Fargs ... fargs) -> Fret {
        //         return f(g(), fhead, fargs ...);
        //     });
        // }

        /*! @brief [3-4] Gret: 参照，Fhead: 値，Fargs: 値
        */
        template <class Fret, class Gret, class Fhead, class ... Fargs>
        static std::function <Fret(Fhead, Fargs ...)> eval(const std::function <Fret(const Gret&, Fhead, Fargs ...)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret(Fhead, Fargs ...)>;

            return result_type([&](Fhead fhead, Fargs ... fargs) -> Fret {
                return f(g(), fhead, fargs ...);
            });
        }

        /*! @brief [3-5] Gret: 値，Fhead: 参照，Fargs: 参照
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs>
        // static std::function <Fret(Fhead&, Fargs& ...)> eval(const std::function <Fret(Gret, Fhead&, Fargs& ...)> &f, const std::function <Gret()> &g)
        // {
        //     using result_type = std::function <Fret(Fhead&, Fargs& ...)>;
        //
        //     return result_type([&](Fhead& fhead, Fargs& ... fargs) -> Fret {
        //         return f(g(), fhead, fargs ...);
        //     });
        // }
        //
        // /*! @brief [3-6] Gret: 参照，Fhead: 参照，Fargs: 値
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs>
        // static std::function <Fret(Fhead&, Fargs ...)> eval(const std::function <Fret(Gret&, Fhead&, Fargs ...)> &f, const std::function <Gret()> &g)
        // {
        //     using result_type = std::function <Fret(Fhead&, Fargs ...)>;
        //
        //     return result_type([&](Fhead& fhead, Fargs ... fargs) -> Fret {
        //         return f(g(), fhead, fargs ...);
        //     });
        // }
        //
        // /*! @brief [3-7] Gret: 参照，Fhead: 値，Fargs: 参照
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs>
        // static std::function <Fret(Fhead, Fargs& ...)> eval(const std::function <Fret(Gret&, Fhead, Fargs& ...)> &f, const std::function <Gret()> &g)
        // {
        //     using result_type = std::function <Fret(Fhead, Fargs& ...)>;
        //
        //     return result_type([&](Fhead fhead, Fargs& ... fargs) -> Fret {
        //         return f(g(), fhead, fargs ...);
        //     });
        // }
        //
        // /*! @brief [3-8] Gret: 参照，Fhead: 参照，Fargs: 参照
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs>
        // static std::function <Fret(Fhead&, Fargs& ...)> eval(const std::function <Fret(Gret&, Fhead&, Fargs& ...)> &f, const std::function <Gret()> &g)
        // {
        //     using result_type = std::function <Fret(Fhead&, Fargs& ...)>;
        //
        //     return result_type([&](Fhead& fhead, Fargs& ... fargs) -> Fret {
        //         return f(g(), fhead, fargs ...);
        //     });
        // }

        /*! @brief [4] fの引数が複数，gの引数がある場合
        */
        /*! @brief [4-1] Gret: 値，Fhead: 値，Fargs: 値，Ghead: 値，Gargs: 値
        */
        template <class Fret, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        static std::function <Fret(Ghead, Gargs ..., Fhead, Fargs ...)> eval(const std::function <Fret(Gret, Fhead, Fargs ...)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(Ghead, Gargs ..., Fhead, Fargs ...)>;

            return result_type([&](Ghead ghead, Gargs ... gargs, Fhead fhead, Fargs ... fargs) -> Fret {
                return f(g(ghead, gargs ...), fhead, fargs ...);
            });
        }

        // /*! @brief [4-2] Gret: 値，Fhead: 値，Fargs: 値，Ghead: 値，Gargs: 参照
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead, Gargs& ..., Fhead, Fargs ...)> eval(const std::function <Fret(Gret, Fhead, Fargs ...)> &f, const std::function <Gret(Ghead, Gargs& ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead, Gargs& ..., Fhead, Fargs ...)>;
        //
        //     return result_type([&](Ghead ghead, Gargs& ... gargs, Fhead fhead, Fargs ... fargs) -> Fret {
        //         return f(g(ghead, gargs ...), fhead, fargs ...);
        //     });
        // }
        //
        // /*! @brief [4-3] Gret: 値，Fhead: 値，Fargs: 値，Ghead: 参照，Gargs: 値
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead&, Gargs ..., Fhead, Fargs ...)> eval(const std::function <Fret(Gret, Fhead, Fargs ...)> &f, const std::function <Gret(Ghead&, Gargs ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead&, Gargs ..., Fhead, Fargs ...)>;
        //
        //     return result_type([&](Ghead& ghead, Gargs ... gargs, Fhead fhead, Fargs ... fargs) -> Fret {
        //         return f(g(ghead, gargs ...), fhead, fargs ...);
        //     });
        // }
        //
        // /*! @brief [4-4] Gret: 値，Fhead: 値，Fargs: 参照，Ghead: 値，Gargs: 値
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead, Gargs ..., Fhead, Fargs& ...)> eval(const std::function <Fret(Gret, Fhead, Fargs& ...)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead, Gargs ..., Fhead, Fargs& ...)>;
        //
        //     return result_type([&](Ghead ghead, Gargs ... gargs, Fhead fhead, Fargs& ... fargs) -> Fret {
        //         return f(g(ghead, gargs ...), fhead, fargs ...);
        //     });
        // }
        //
        // /*! @brief [4-5] Gret: 値，Fhead: 参照，Fargs: 値，Ghead: 値，Gargs: 値
        // */
        // template <class Fret, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        // static std::function <Fret(Ghead, Gargs ..., Fhead&, Fargs ...)> eval(const std::function <Fret(Gret, Fhead&, Fargs ...)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        // {
        //     using result_type = std::function <Fret(Ghead, Gargs ..., Fhead&, Fargs ...)>;
        //
        //     return result_type([&](Ghead ghead, Gargs ... gargs, Fhead& fhead, Fargs ... fargs) -> Fret {
        //         return f(g(ghead, gargs ...), fhead, fargs ...);
        //     });
        // }

        /*! @brief [4-6] Gret: 参照，Fhead: 値，Fargs: 値，Ghead: 値，Gargs: 値
        */
        template <class Fret, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        static std::function <Fret(Ghead, Gargs ..., Fhead, Fargs ...)> eval(const std::function <Fret(const Gret&, Fhead, Fargs ...)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(Ghead, Gargs ..., Fhead, Fargs ...)>;

            return result_type([&](Ghead ghead, Gargs ... gargs, Fhead fhead, Fargs ... fargs) -> Fret {
                return f(g(ghead, gargs ...), fhead, fargs ...);
            });
        }
    };

    template <>
    struct Composite <1> {
        /*! @brief [1] fの引数が2つ，gの引数がない場合
        */
        template <class Fret, class F1, class Gret>
        static std::function <Fret(F1)> eval(const std::function <Fret(F1, Gret)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret(F1)>;

            return result_type([&](F1 f1) -> Fret {
                return f(f1, g());
            });
        }

        template <class Fret, class F1, class Gret>
        static std::function <Fret(F1)> eval(const std::function <Fret(F1, const Gret&)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret(F1)>;

            return result_type([&](F1 f1) -> Fret {
                return f(f1, g());
            });
        }

        /*! @brief [2] fの引数が2つ，gの引数がある場合
        */
        template <class Fret, class F1, class Gret, class Ghead, class ... Gargs>
        static std::function <Fret(F1, Ghead, Gargs ...)> eval(const std::function <Fret(F1, Gret)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(F1, Ghead, Gargs ...)>;

            return result_type([&](F1 f1, Ghead ghead, Gargs ... gargs) -> Fret {
                return f(f1, g(ghead, gargs ...));
            });
        }

        template <class Fret, class F1, class Gret, class Ghead, class ... Gargs>
        static std::function <Fret(F1, Ghead, Gargs ...)> eval(const std::function <Fret(F1, const Gret&)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(F1, Ghead, Gargs ...)>;

            return result_type([&](F1 f1, Ghead ghead, Gargs ... gargs) -> Fret {
                return f(f1, g(ghead, gargs ...));
            });
        }

        /*! @brief [3] fの引数が3つ以上，gの引数がない場合
        */
        template <class Fret, class F1, class Gret, class Fhead, class ... Fargs>
        static std::function <Fret(F1, Fhead, Fargs ...)> eval(const std::function <Fret(F1, Gret, Fhead, Fargs ...)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret(F1, Fhead, Fargs ...)>;

            return result_type([&](F1 f1, Fhead fhead, Fargs ... fargs) -> Fret {
                return f(f1, g(), fhead, fargs ...);
            });
        }

        template <class Fret, class F1, class Gret, class Fhead, class ... Fargs>
        static std::function <Fret(F1, Fhead, Fargs ...)> eval(const std::function <Fret(F1, const Gret&, Fhead, Fargs ...)> &f, const std::function <Gret()> &g)
        {
            using result_type = std::function <Fret(F1, Fhead, Fargs ...)>;

            return result_type([&](F1 f1, Fhead fhead, Fargs ... fargs) -> Fret {
                return f(f1, g(), fhead, fargs ...);
            });
        }

        /*! @brief [4] fの引数が3つ以上，gの引数がある場合
        */
        template <class Fret, class F1, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        static std::function <Fret(F1, Ghead, Gargs ..., Fhead, Fargs ...)> eval(const std::function <Fret(F1, Gret, Fhead, Fargs ...)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(F1, Ghead, Gargs ..., Fhead, Fargs ...)>;

            return result_type([&](F1 f1, Ghead ghead, Gargs ... gargs, Fhead fhead, Fargs ... fargs) -> Fret {
                return f(f1, g(ghead, gargs ...), fhead, fargs ...);
            });
        }

        template <class Fret, class F1, class Gret, class Fhead, class ... Fargs, class Ghead, class ... Gargs>
        static std::function <Fret(F1, Ghead, Gargs ..., Fhead, Fargs ...)> eval(const std::function <Fret(F1, const Gret&, Fhead, Fargs ...)> &f, const std::function <Gret(Ghead, Gargs ...)> &g)
        {
            using result_type = std::function <Fret(F1, Ghead, Gargs ..., Fhead, Fargs ...)>;

            return result_type([&](F1 f1, Ghead ghead, Gargs ... gargs, Fhead fhead, Fargs ... fargs) -> Fret {
                return f(g(ghead, gargs ...), fhead, fargs ...);
            });
        }
    };
}

#endif
