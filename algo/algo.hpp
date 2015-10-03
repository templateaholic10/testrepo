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
#include "../lab/util.hpp"

namespace util {
    // ランダムアクセスシーケンスを非破壊的にシャッフルする関数．
    template <typename T>
    std::vector <T> Fisher_Yates_shuffle(const std::vector <T> &seq)
    {
        const std::size_t size = seq.size();

        // 添字シーケンスの作成．
        std::vector <int> indexes(size);
        std::iota(indexes.begin(), indexes.end(), 0);

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
    std::array <T, num> Fisher_Yates_shuffle(const std::array <T, num> &seq)
    {
        // 添字シーケンスの作成．
        std::array <int, num> indexes;
        std::iota(indexes.begin(), indexes.end(), 0);

        // 乱数の設定．
        std::random_device rnd;
        std::mt19937       mt(rnd());

        // 添字シーケンスのシャッフル．
        for (size_t i = 0; i < num; i++) {
            std::uniform_int_distribution <> rnd_index(i, num - 1);
            int                              index = rnd_index(mt);
            std::swap(indexes[i], indexes[index]);
        }

        // 結果シーケンスのセット．
        std::array <T, num> result;
        for (size_t i = 0; i < num; i++) {
            result[i] = seq[indexes[i]];
        }

        return result;
    }

    void test_Fisher_Yates_shuffle()
    {
        constexpr std::size_t size = 10;

        std::vector <int> v(size);
        std::iota(v.begin(), v.end(), 0);
        _DISPLAY_SEQ(v)
        v = Fisher_Yates_shuffle(v);
        _DISPLAY_SEQ(v)

        std::array <int, size> a;
        std::iota(a.begin(), a.end(), 0);
        _DISPLAY_SEQ(a)
        a = Fisher_Yates_shuffle(a);
        _DISPLAY_SEQ(a)
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
        std::size_t Playertoi(const Player player)
        {
            if (player == Player::Alice) {
                return 0;
            } else {
                return 1;
            }
        }

        // プレイヤーを文字列に変換する関数．
        std::string str(const Player player)
        {
            if (player == Player::Alice) {
                return "Alice";
            } else {
                return "Bob";
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
            bool  is_front;

            static int serial;

            Card()
                : color(itoColor(serial % 2)), number(serial / 2), is_front(false)
            {
                serial++;
            }

            Card(Color color_, int number_, bool is_front_)
                : color(color_), number(number_), is_front(is_front_)
            {
            }

            bool operator<(const Card &rhs) const
            {
                return (number == rhs.number) ? (color < rhs.color) : (number < rhs.number);
            }

            bool operator==(const Card &rhs) const
            {
                return (color == rhs.color) && (number == rhs.number);
            }

            friend std::ostream&operator<<(std::ostream &os, const Card &card);
        };

        int Card::serial = 0;

        // 表→黒: [ 0]，白: ( 0)
        // 裏→黒: [  ]，白: (  )
        // 　　黒: * 0]，白: * 0)
        void display_card(const Card &card, bool open=false, std::ostream &os=std::cout)
        {
            if (card.color == Color::Black) {
                if (card.is_front) {
                    os << "[" << std::setw(2) << card.number << "]";
                } else {
                    if (open) {
                        os << "*" << std::setw(2) << card.number << "]";
                    } else {
                        os << "[  ]";
                    }
                }
            } else {
                if (card.is_front) {
                    os << "(" << std::setw(2) << card.number << ")";
                } else {
                    if (open) {
                        os << "*" << std::setw(2) << card.number << ")";
                    } else {
                        os << "(  )";
                    }
                }
            }
        }

        // 普通に出力する場合は透視しない．
        std::ostream&operator<<(std::ostream &os, const Card &card)
        {
            display_card(card, false, os);

            return os;
        }

        // 1ターンの履歴を表す構造体．
        struct Guess {
            Player player;
            int    place;
            Card   card;
            bool   correctness;
            bool   cont_turn;
        };

        // 盤面の全情報を格納する構造体．
        struct Board {
            State                               state; // 状態
            static constexpr std::size_t        cards_num = 24;
            static std::array <Card, cards_num> deck;                 // 山札
            // 配列の前が山札の上．
            size_t                            deck_pos;
            static constexpr std::size_t      start_hand_num = 5;
            std::array <std::deque <Card>, 2> hands;   // 手札
            std::deque <Guess>                history;  // 履歴

            Board();
            void init();
            Card pop_deck() const;
            bool guess(Player player, int place, Card card);
            void take(Player player, Card card, bool is_front);
        };

        std::array <Card, Board::cards_num> Board::deck = std::array <Card, Board::cards_num>();

        void Board::init()
        {
            deck     = util::Fisher_Yates_shuffle(deck);
            deck_pos = 0;
            for (size_t i = 0; i < 2; i++) {
                hands[i].clear();
                for (size_t deal = 0; deal < start_hand_num; deal++) {
                    hands[i].push_back(deck[deck_pos]);
                    deck_pos++;
                }
                std::sort(hands[i].begin(), hands[i].end());
            }
            history.clear();
            state = State::turn_Alice;
        }

        Board::Board()
        {
            init();
        }

        // 手札を確認する関数．
        void display_hand(const Board &board, const Player player, bool open=false, std::ostream &os=std::cout)
        {
            for (auto card : board.hands[Playertoi(player)]) {
                display_card(card, open, os);
            }
            os << std::endl;
        }

        // 盤面を確認する関数．
        void display_board(const Board &board, bool open=false, std::ostream &os=std::cout)
        {
            display_hand(board, Player::Alice, open, os);
            display_hand(board, Player::Bob, open, os);
        }

        // 山札を確認する関数．
        void display_deck(const Board &board, bool open=false, std::ostream &os=std::cout)
        {
            std::for_each(board.deck.begin(), board.deck.end(), [&](const Card &card) {
                display_card(card, open, os);
            });
            os << std::endl;
            const std::string card_space = "    ";
            for (size_t i = 0; i < board.deck_pos; i++) {
                os << card_space;
            }
            os << " top";
            for (size_t i = 0; i < Board::cards_num - board.deck_pos - 2; i++) {
                os << card_space;
            }
            os << " btm";
            os << std::endl;
        }

        // 手札を正しく並べる関数．
        void sort_hands(Board &board)
        {
            for (auto hand : board.hands) {
                std::sort(hand.begin(), hand.end());
            }
        }

        // 山札の一番上を見る関数．
        Card Board::pop_deck() const
        {
            return deck[deck_pos];
        }

        // 予想する関数．
        // player"の"手札を予想する．
        bool Board::guess(Player player, int place, Card card)
        {
            return hands[Playertoi(player)][place] == card;
        }

        // カードを手札に加える関数．
        // 表裏を指定する．
        void Board::take(Player player, Card card, bool is_front)
        {
            auto pos = std::lower_bound(hands[Playertoi(player)].begin(), hands[Playertoi(player)].end(), card);
            card.is_front = is_front;
            hands[Playertoi(player)].insert(pos, card);
        }
    }

    namespace game {
        // 解答を表す構造体．
        struct Guess {
            int          place;
            system::Card card;
        };

        // CPUを表す構造体．
        template <class T>
        class CPU {
            Guess guess(const system::Board& board);
            bool one_more(const system::Board& board);
        };

        // ゲームを表すクラス．
        template <class Alice, class Bob>
        class Game {
        private:
            struct setting {
            };

        public:
            void set();
            void take_turn();
        };
    }

    namespace test {
        void game()
        {
            system::Board board = system::Board();
            system::display_board(board);
            system::display_deck(board);
        }
    }
}

#endif
