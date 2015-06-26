#ifndef PROTOARRAY
#define PROTOARRAY

#include <iostream>
#include <cstddef>
#include <bitset>
#include <boost/optional.hpp>
#include "util.hpp"

namespace protoarray {
    // virtualなconstexpr関数を書くことはできない．

    // バイナリ上の長さnの配列
    // ビット""列""として見ているので1-origin
    template <std::size_t length>
    class Protobitarray
    {
    public:
        using length_t  = util::container_t <length>;
        using element_t = length_t;   // 要素（2値）
        // lengthが収まる型を用いる．
        using time_t     = length_t;  // 出現回数
        using position_t = length_t;  // 出現位置（1-origin）
    public:
        constexpr Protobitarray()
        {
        }

        constexpr Protobitarray(const char org_array[length])
        {
        }

        constexpr Protobitarray(const unsigned long org_array)
        {
        }

        // ~Protobitarray()                      = default;
        // Protobitarray(const Protobitarray&)           = delete;
        // Protobitarray&operator=(const Protobitarray&) = delete;
        //
        // Protobitarray(Protobitarray&&)           = delete;
        // Protobitarray&operator=(Protobitarray&&) = delete;

        constexpr unsigned long access(const position_t index) const;

        constexpr unsigned long rank(const element_t a, const position_t index) const;

        constexpr unsigned long select(const element_t a, const time_t order) const;

        constexpr size_t        alphabet_size() const;
        constexpr size_t        size() const;
        std::string             to_string() const;
        constexpr unsigned long invalid_value() const;
        std::string             str() const;
        std::string             superblock_rank() const;
        std::string             block_rank() const;
        std::string             lookuptable() const;

        friend std::ostream    &operator<<(std::ostream &os, const Protobitarray <length> &pb);
    };

    // アルファベット[0, alphabet_num)上の長さnの配列
    template <int alphabet_num, std::size_t length>
    class Protoarray
    {
        using length_t   = util::container_t <length>;
        using alphabet_t = util::container_t <alphabet_num - 1>;
        using element_t  = alphabet_t;
        using time_t     = length_t;
        using position_t = length_t;
    public:
        constexpr Protoarray()
        {
        }

        // ~Protoarray()                   = default;
        // Protoarray(const Protoarray&)           = delete;
        // Protoarray&operator=(const Protoarray&) = delete;
        //
        // Protoarray(Protoarray&&)                                    = delete;
        // Protoarray                         &operator=(Protoarray&&) = delete;

        unsigned long access(const position_t index) const;

        unsigned long rank(const element_t a, const position_t index) const;

        const
        unsigned long           select(const element_t a, const time_t order) const;

        constexpr size_t        alphabet_size() const;
        constexpr size_t        size() const;
        std::string             to_string() const;
        constexpr unsigned long invalid_value() const;
        std::string             str() const;
        std::string             superblock_rank() const;
        std::string             block_rank() const;
        std::string             lookuptable() const;

        friend std::ostream    &operator<<(std::ostream &os, const Protobitarray <length> &pb);
    };

}

#include "detail/protoarray.hpp"

#endif
