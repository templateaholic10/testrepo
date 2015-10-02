#ifndef ALGO
#define ALGO

#include <iostream>
#include <utility>
#include <cassert>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <stack>
#include <algorithm>
#include <numeric>
#include <random>

namespace util {
    // ランダムアクセスシーケンスを非破壊的にシャッフルする関数．
    template <typename T>
    std::vector <T> Fisher_Yates_shuffle(const std::vector <T> seq)
    {
        const std::size_t size = seq.size();

        // 添字シーケンスの作成．
        std::vector <int> indexes(size);
        std::iota(indexes.begin(), indexes.end(), 1);

        // 乱数の設定．
        std::random_device rnd;
        std::mt19937       mt(rnd());

        // 添字シーケンスのシャッフル．
        for (size_t i = 0; i < size; i++) {
            std::uniform_int_distribution <> rnd_index(i, size - 1);
            int                              index = rnd_index(mt);
            std::swap(indexes[i], indexes[index]);
        }

        // 結果シーケンスのセット．
        std::vector <T> result(size);
        for (size_t i = 0; i < size; i++) {
            result[i] = seq[indexes[i]];
        }

        return result;
    }

    template <typename T, std::size_t num>
    std::array <T, num> Fisher_Yates_shuffle(const std::array <T, num> seq)
    {
        const std::size_t size = seq.size();

        // 添字シーケンスの作成．
        std::array <int, num> indexes;
        std::iota(indexes.begin(), indexes.end(), 1);

        // 乱数の設定．
        std::random_device rnd;
        std::mt19937       mt(rnd());

        // 添字シーケンスのシャッフル．
        for (size_t i = 0; i < size; i++) {
            std::uniform_int_distribution <> rnd_index(i, size - 1);
            int                              index = rnd_index(mt);
            std::swap(indexes[i], indexes[index]);
        }

        // 結果シーケンスのセット．
        std::array <T, num> result(size);
        for (size_t i = 0; i < size; i++) {
            result[i] = seq[indexes[i]];
        }

        return result;
    }
}

namespace algo {
    namespace system {
        // algoのシステムを実装する．

        // プレイヤーを表す列挙型．
        enum class Player {
            Alice,
            Bob,
        };

        // プレイヤーを数字に変換する関数．
        std::size_t numerize(const Player player)
        {
            if (player == Player::Alice) {
                return 0;
            } else {
                return 1;
            }
        }

        // 盤面の状態を表す列挙型．
        enum class State {
            turn_Alice,
            turn_Bob,
            win_Alice,
            win_Bob,
        };

        // 色を表す列挙型．
        enum class Color {
            Black,
            White,
        };

        Color itoColor(int num)
        {
            assert(num == 0 || num == 1);
            if (num == 0) {
                return Color::Black;
            } else {
                return Color::White;
            }
        }

        // カードを表す構造体．
        struct Card {
            Color color;
            int   number;
            bool  open;

            static int serial;

            Card()
                : color(itoColor(serial % 2)), number(serial / 2), open(false)
            {
                serial++;
            }

            bool operator<(const Card &rhs) const
            {
                return (number == rhs.number) ? (color < rhs.color) : (number < rhs.number);
            }

            friend std::ostream&operator<<(std::ostream &os, const Card &card);
        };

        int Card::serial = 0;

        std::ostream&operator<<(std::ostream &os, const Card &card)
        {
            if (card.color == Color::Black) {
                if (card.open) {
                    os << "[" << std::setw(2) << card.number << "]";
                } else {
                    os << "[  ]";
                }
            } else {
                if (card.open) {
                    os << "(" << std::setw(2) << card.number << ")";
                } else {
                    os << "(  )";
                }
            }
        }

        void see_through(std::ostream &os, const Card &card)
        {
            if (card.color == Color::Black) {
                os << "[" << std::setw(2) << card.number << "]";
            } else {
                os << "(" << std::setw(2) << card.number << ")";
            }
        }

        // 盤面の全情報を格納する構造体．
        struct Board {
            State                              state; // 状態
            static constexpr std::size_t cards_num = 24;
            static std::array <Card, cards_num>                 deck; // 山札
            size_t deck_pos;
            static constexpr std::size_t start_hand_num = 5;
            std::array <std::deque <Card>, 2> hands; // 手札

            Board()
                : state(State::turn_Alice), deck_pos(0)
            {
                deck         = util::Fisher_Yates_shuffle(deck);
                for (auto hand : hands) {
                    hand.clear();
                    for (size_t i = 0; i < start_hand_num; i++) {
                        hand.push_back(deck[deck_pos]);
                        deck_pos++;
                    }
                    std::sort(hand.begin(), hand.end());
                }
            }
        };

        std::array<Card, Board::cards_num> Board::deck = std::array<Card, Board::cards_num>();

        // 手札を確認する関数．
        void display(const Board &board, const Player player, bool open=false)
        {
            if (open) {
                for (auto card : board.hands[numerize(player)]) {
                    see_through(std::cout, card);
                }
            } else {
                for (auto card : board.hands[numerize(player)]) {
                    std::cout << card;
                }
            }
            std::cout << std::endl;
        }

        // 手札を正しく並べる関数．
        void sort_hands(Board &board)
        {
            for (auto hand : board.hands) {
                std::sort(hand.begin(), hand.end());
            }
        }

        // 山札の一番上を見る関数．
        Card pop_deck(const Board &board)
        {
            return board.deck[board.deck_pos];
        }

        // 手を進める関数．
        void move(Board &board)
        {
            
        }
    }
}

#endif
