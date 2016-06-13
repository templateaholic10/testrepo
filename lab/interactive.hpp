/*! @file
    @brief 対話型環境の基底クラス
    @author templateaholic10
    @date 11/20
*/
#ifndef INTERACTIVE_HPP
#define INTERACTIVE_HPP

#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <util>

namespace util {
    /*! @class
        @brief コマンドを表す構造体．()演算子でコールする
    */
    struct Command {
        const char                   name;
        const std::string            description;
        const std::function <void()> body;

        Command() = delete;
        Command(const char name_, const std::string &description_, const std::function <void()> &body_)
            : name(name_), description(description_), body(body_)
        {
        }

        void operator()() const
        {
            body();
        }
    };

    /*! @class
        @brief 対話型環境の基底クラス．showで起動する
    */
    class Interactive {
    protected:
        const std::string     name;
        std::vector <Command> commands;
        std::istream         &is;
        std::ostream         &os;
        std::string           dirname;
    public:
        Interactive() = delete;
        Interactive(const std::string name_, std::istream &is_=std::cin, std::ostream &os_=std::cout)
            : name(name_), is(is_), os(os_)
        {
            commands.push_back(Command('q', "quit", std::function <void(void)>([]() {
            })));
        }

        void display_commands() const
        {
            os << "[COMMAND]" << std::endl;
            for (auto command : commands) {
                os << command.name << ": " << command.description << std::endl;
            }
        }

        void show()
        {
            os << util::Repeat("-", 20) << std::endl;
            os << name << std::endl;
            os << std::endl;
            os << "input project name" << std::endl;
            os << ">";
            is >> dirname;
            while (true) {
                os << std::endl;
                display_commands();
                os << std::endl;
                os << "input command" << std::endl;
                os << ">";
                char ch;
                is >> ch;
                auto checker = [&](const Command &command) {
                                   return command.name == ch;
                               };
                auto it      = std::find_if(commands.begin(), commands.end(), checker);
                if (it == commands.end()) {
                    continue;
                } else if (ch == 'q') {
                    os << util::Repeat("-", 20) << std::endl;

                    return;
                } else {
                    (*it)();
                }
            }
        }
    };
}

#endif
