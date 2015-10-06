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
#include <memory>
#include <algorithm>
#include <numeric>
#include <random>
#include "../lab/util.hpp"

namespace util {
    // デバッグ用出力コメント．
    class Comment {
    private:
        static std::string header;
        const std::string  content;
    public:
        Comment(const std::string &content_)
            : content(content_)
        {
        }

        friend std::ostream&operator<<(std::ostream &os, const Comment &comment);
    };

    std::string Comment::header = "?? ";

    std::ostream&operator<<(std::ostream &os, const Comment &comment)
    {
        os << comment.header << comment.content << std::endl;

        return os;
    }

    // y/nをtrue/falseに変換する関数．
    // yまたはY以外はfalse．
    bool ctob(const char c)
    {
        return c == 'y' || c == 'Y';
    }

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

        // プレイサイドを表す列挙型．
        enum class Playside {
            Alice,
            Bob,
        };

        // プレイサイドを反転させる関数．
        Playside invert(const Playside playside)
        {
            if (playside == Playside::Alice) {
                return Playside::Bob;
            } else {
                return Playside::Alice;
            }
        }

        // プレイサイドを数字に変換する関数．
        std::size_t Playsidetoi(const Playside playside)
        {
            if (playside == Playside::Alice) {
                return 0;
            } else {
                return 1;
            }
        }

        // プレイサイドを文字列に変換する関数．
        std::string str(const Playside playside)
        {
            if (playside == Playside::Alice) {
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

        // 普通に出力する場合は透視する．
        std::ostream&operator<<(std::ostream &os, const Card &card)
        {
            display_card(card, true, os);

            return os;
        }

        // 予想を表す構造体．
        struct Guess {
            size_t place;
            int    number;

            struct Header {
                static const std::string __str__;

                friend std::ostream& operator<<(std::ostream& os, const Guess::Header &header);
            };

            Guess(size_t place_, int number_)
                : place(place_), number(number_)
            {
            }
            friend std::ostream& operator<<(std::ostream& os, const Guess &guess);
        };

        const std::string Guess::Header::__str__ = "place number";

        std::ostream& operator<<(std::ostream& os, const Guess::Header &header)
        {
            os << Guess::Header::__str__;
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const Guess &guess)
        {
            os << std::setw(5) << guess.place << " " << std::setw(6) << guess.number;
            return os;
        }

        // 盤面の全情報を格納する構造体．
        struct Board {
            State                               state; // 状態
            static constexpr std::size_t        cards_num = 24;
            static std::array <Card, cards_num> deck;                 // 山札
            // 配列の前が山札の上．
            size_t                            deck_pos;
            static constexpr std::size_t default_start_hand_num = 4;
            const std::size_t      start_hand_num;
            std::array <std::deque <Card>, 2> hands;   // 手札

            Board();
            Board(std::size_t start_hand_num_);
            void               init();

            std::deque <Card> &hand(Playside playside);

            Card pop_deck() const;
            bool check(Playside playside, Guess guess) const;
            void turn(Playside playside, size_t place, bool to_front);
            bool full_open(Playside playside) const;
            void take(Playside playside, Card card, bool is_front);
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
            state = State::turn_Alice;
        }

        Board::Board(std::size_t start_hand_num_)
        : start_hand_num(start_hand_num_)
        {
            init();
        }

        Board::Board()
        : start_hand_num(default_start_hand_num)
        {
            init();
        }

        // いちいち変換するのが面倒なので参照返しでラップ．
        std::deque <Card>&Board::hand(Playside playside)
        {
            return hands[system::Playsidetoi(playside)];
        }

        // 手札を確認する関数．
        void display_hand(const Board &board, const Playside playside, bool open=false, std::ostream &os=std::cout)
        {
            for (auto card : board.hands[Playsidetoi(playside)]) {
                display_card(card, open, os);
            }
            os << std::endl;
        }

        // 盤面を確認する関数．
        void display_board(const Board &board, bool open=false, std::ostream &os=std::cout)
        {
            display_hand(board, Playside::Alice, open, os);
            display_hand(board, Playside::Bob, open, os);
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

        // 確認する関数．
        // playside"の"手札の予想を確認する．
        bool Board::check(Playside playside, Guess guess) const
        {
            assert(0 <= guess.place && guess.place < hands[Playsidetoi(playside)].size());

            return hands[Playsidetoi(playside)][guess.place].number == guess.number;
        }

        // カードの向きを変える関数．
        void Board::turn(Playside playside, size_t place, bool to_front)
        {
            hand(playside)[place].is_front = to_front;
        }

        // 手札がすべて開いているかを調べる関数．
        bool Board::full_open(Playside playside) const
        {
            for (auto card : hands[Playsidetoi(playside)]) {
                if (!card.is_front) {
                    return false;
                }
            }

            return true;
        }

        // カードを手札に加える関数．
        // 表裏を指定する．
        void Board::take(Playside playside, Card card, bool is_front)
        {
            assert(0 <= deck_pos && deck_pos < Board::cards_num - 1);
            auto pos = std::lower_bound(hands[Playsidetoi(playside)].begin(), hands[Playsidetoi(playside)].end(), card);
            card.is_front = is_front;
            hands[Playsidetoi(playside)].insert(pos, card);
            deck_pos++;
        }
    }

    namespace game {
        // 基底クラス．
        class Character {
        public:
            const std::string      name;
            const system::Playside playside;
        public:
            Character(const std::string &name_, system::Playside playside_)
                : name(name_), playside(playside_)
            {
            }

            virtual system::Guess guess(const system::Board &board) const    = 0;
            virtual bool          one_more(const system::Board &board) const = 0;
        };

        // 人クラス．
        class Person : public Character {
        private:
            std::istream &is;
            std::ostream &os;
        public:
            Person(const std::string &name_, system::Playside playside_, std::istream &is_, std::ostream &os_)
                : Character(name_, playside_), is(is_), os(os_)
            {
            }

            system::Guess guess(const system::Board &board) const;
            bool          one_more(const system::Board &board) const;
        };

        system::Guess Person::guess(const system::Board &board) const
        {
            os << name << ">" << std::endl;

            // 報告
            os << "     ";
            for (size_t i = 0; i < system::Board::cards_num/2; i++) {
                os << " " << std::setw(2) << i << " ";
            }
            os << std::endl;
            os << "Opp: ";
            display_hand(board, system::invert(playside), false, os);
            os << "You: ";
            display_hand(board, playside, true, os);
            os << "Drew: ";
            os << board.pop_deck();
            os << std::endl;

            // 入力
            size_t place;
            int    number;

            os << "Guess a number of any face-down card." << std::endl;
            os << "To check history, input \"h\"." << std::endl;
            os << "place(0-" << board.hands[system::Playsidetoi(system::invert(playside))].size() << "): ";
            while (true) {
                is >> place;
                if (board.hands[system::Playsidetoi(system::invert(playside))][place].is_front) {
                    os << "Already open." << std::endl;
                    os << "place(0-" << board.hands[system::Playsidetoi(system::invert(playside))].size() << "): ";
                } else {
                    break;
                }
            }
            os << "number(0-11): ";
            is >> number;

            os << std::endl;

            return system::Guess(place, number);
        }

        bool Person::one_more(const system::Board &board) const
        {
            // 報告
            os << "Great guess!" << std::endl;
            os << "Drew: ";
            os << board.pop_deck();
            os << std::endl;

            // 入力
            char c;

            os << "Guess more?" << std::endl;
            os << "y/n: ";
            is >> c;

            os << std::endl;

            return util::ctob(c);
        }

        // ランダムクラス．
        class Randy : public Character {
        private:
        public:
            Randy(const std::string &name_, system::Playside playside_)
                : Character(name_, playside_)
            {
            }

            system::Guess guess(const system::Board &board) const;
            bool          one_more(const system::Board &board) const;
        };

        system::Guess Randy::guess(const system::Board &board) const
        {
            // 入力
            size_t place;
            int    number;

            std::random_device rnd;
            std::mt19937       mt(rnd());

            std::uniform_int_distribution <> rnd_place(0, board.hands[system::Playsidetoi(system::invert(playside))].size() - 1);
            while (true) {
                place = rnd_place(mt);
                if (!board.hands[system::Playsidetoi(system::invert(playside))][place].is_front) {
                    break;
                }
            }

            std::uniform_int_distribution <> rnd_number(0, 11);
            number = rnd_number(mt);

            return system::Guess(place, number);
        }

        bool Randy::one_more(const system::Board &board) const
        {
            return true;
        }

        // 履歴を表す構造体．
        struct Record {
            system::Guess guess;
            bool          correctness;
            bool          one_more;

            struct Header {
                static const std::string __str__;

                friend std::ostream& operator<<(std::ostream& os, const Record::Header& header);
            };

            Record(system::Guess guess_, bool correctness_, bool one_more_)
                : guess(guess_), correctness(correctness_), one_more(one_more_)
            {
            }

            friend std::ostream& operator<<(std::ostream& os, const Record &record);
        };

        const std::string Record::Header::__str__ = "result one_more";

        std::ostream& operator<<(std::ostream& os, const Record::Header& header)
        {
            os << system::Guess::Header() << " " << Record::Header::__str__;
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const Record &record)
        {
            os << record.guess << " " << (record.correctness ? "  true  " : "  false ") << (record.one_more ? "  true  " : "  false ");
            return os;
        }

        // ゲームを表すクラス．
        class Game {
        private:
            system::Board                               board;
            std::array <std::unique_ptr <Character>, 2> characters;
            std::deque <Record>                         history;
        public:
            Game(std::unique_ptr <Character> &&Alice_, std::unique_ptr <Character> &&Bob_)
                : board(), characters(std::array <std::unique_ptr <Character>, 2>(
            {
                std::move(Alice_), std::move(Bob_)
            }))
            {
            }

            std::unique_ptr <Character> &character(system::Playside playside);

            bool                         take_turn(system::Playside playside);
            void                         main();
        };

        std::unique_ptr <Character>&Game::character(system::Playside playside)
        {
            return characters[system::Playsidetoi(playside)];
        }

        // ターンをこなす破壊的関数．
        // ゲーム終了時にはtrue，それ以外にはfalseを返す．
        bool Game::take_turn(system::Playside playside)
        {
            bool cont      = true;
            bool full_open = false;
            do {
                // 予想．
                auto guess = character(playside)->guess(board);

                // 判定．
                bool result = board.check(system::invert(playside), guess);

                bool one_more = false;

                if (result) {
                    // 正解した場合．
                    // オープン．
                    board.turn(system::invert(playside), guess.place, true);

                    // ゲームの終了判定．
                    full_open = board.full_open(system::invert(playside));
                    if (full_open) {
                        cont = false;
                    } else {
                        // アタックかディフェンスか．
                        one_more = character(playside)->one_more(board);

                        if (!one_more) {
                            // ディフェンスの場合．
                            cont = false;
                            board.take(playside, board.pop_deck(), false);
                        }
                    }
                } else {
                    // 外れた場合．
                    cont = false;
                    board.take(playside, board.pop_deck(), true);
                }
                // 履歴．
                history.push_back(Record(guess, result, one_more));
            } while (cont);

            return full_open;
        }

        void Game::main()
        {
            bool end_flag = false;
            do {
                end_flag = take_turn(system::Playside::Alice);
                end_flag = take_turn(system::Playside::Bob);
            } while (!end_flag);
        }
    }

    namespace test {
        void PvP()
        {
            game::Game G = game::Game(std::unique_ptr <game::Character>(new game::Person("Akari", system::Playside::Alice, std::cin, std::cout)), std::unique_ptr <game::Character>(new game::Person("Sumire", system::Playside::Bob, std::cin,
                                                                                                                                                                                                     std::cout)));
            G.main();
        }

        void PvC()
        {
            game::Game G = game::Game(std::unique_ptr <game::Character>(new game::Randy("Akari", system::Playside::Alice)), std::unique_ptr <game::Character>(new game::Person("Me", system::Playside::Bob, std::cin,
                                                                                                                                                                                                     std::cout)));
            G.main();
        }
    }
}

#endif
