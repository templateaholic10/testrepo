#ifndef WAVELETMATRIX
#define WAVELETMATRIX

#include "unionbitarray.hpp"

namespace waveletmatrix {
    template <std::size_t alphabet_num, std::size_t length>
    class Waveletmatrix
    {
    public:
        using length_t   = typename protoarray::Protoarray <alphabet_num, length>::length_t;
        using alphabet_t = typename protoarray::Protoarray <alphabet_num, length>::alphabet_t;
        using element_t  = typename protoarray::Protoarray <alphabet_num, length>::element_t;
        using time_t     = typename protoarray::Protoarray <alphabet_num, length>::time_t;
        using position_t = typename protoarray::Protoarray <alphabet_num, length>::position_t;
    public:
        constexpr Waveletmatrix();
        constexpr Waveletmatrix(const char org_array[length]);
        constexpr Waveletmatrix(const sprout::array<unsigned long, length>& org_array);
        Waveletmatrix(const std::array<unsigned long, length>& org_array);
        // ~Waveletmatrix()                                  = default;
        // Waveletmatrix(const Waveletmatrix&)           = default;
        // Waveletmatrix&operator=(const Waveletmatrix&) = default;

        // Waveletmatrix(Waveletmatrix&&)           = default;
        // Waveletmatrix&operator=(Waveletmatrix&&) = default;

        // (bitwise rank * 1) * matrix_depth
        constexpr unsigned long access(const position_t index) const;

        // (bitwise rank * 2) * matrix_depth
        constexpr unsigned long rank(const element_t a, const position_t index) const;

        // (bitwise rank * 1 + bitwise select * 1) * matrix_depth
        constexpr unsigned long select(const element_t a, const time_t order) const;

        constexpr size_t        alphabet_size() const;
        constexpr size_t        size() const;
        std::string             to_string() const;
        constexpr unsigned long invalid_value() const;
        std::string             str() const;
        std::string             superblock_rank() const;
        std::string             block_rank() const;
        std::string             lookuptable() const;

        template <std::size_t alphabet_num1, std::size_t length1>
        friend std::ostream&operator<<(std::ostream &os, const Waveletmatrix <alphabet_num1, length1> &wm);
    private:
        constexpr void build();

        static constexpr alphabet_t matrix_depth = util::lg(alphabet_num);

        // s = alphabet_num，n = lengthとする．
        // もともとの配列 O(n lg s) space
        sprout::array<element_t, length> _org_array;
        // waveletmatrix O(n lg s) space
        sprout::array<unionbitarray::Unionbitarray<length>, matrix_depth> _bitmatrix;
        // 各レベルの0の数（区切り）zn O((lg n)(lg s)) space
        sprout::array<length_t, matrix_depth> _partition;
    };
}

#include "detail/waveletmatrix.hpp"

#endif
