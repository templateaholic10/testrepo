#include <iostream>
#include <fstream>
#include <util>
#include <picojson>

int main()
{
    picojson::value v;
    std::ifstream fin("sample.json");
    if (fin.fail()) {
        std::cerr << "file open error!" << std::endl;
        return 1;
    }
    fin >> v;
    fin.close();

    std::string err = picojson::get_last_error();

    if (!v.is<picojson::object>()) {
        std::cerr << "JSON is not an object!" << std::endl;
        return 2;
    }

    // オブジェクトや配列の要素はすべてpicojson::valueなのでgetで取り出す必要がある
    // ただしオブジェクトのキーはconst std::string
    // picojson::valueのまま出力すると文字列として出力される
    const auto &obj = v.get<picojson::object>();

    const auto &Aikatsu = obj.at("Aikatsu").get<picojson::object>();
    for (auto elem : Aikatsu) {
        std::cout << elem.first << ": " << elem.second.get<std::string>() << std::endl;
    }

    const auto &Words = obj.at("Words").get<picojson::array>();
    for (auto elem : Words) {
        std::cout << elem.get<std::string>() << std::endl;
    }

    const auto &Others = obj.at("Others").get<picojson::array>();
    const auto &_1 = Others.at(0).get<double>();
    std::cout << _1 << std::endl;
    const auto &_true = Others.at(1).get<bool>();
    std::cout << _true << std::endl;
    assert(Others.at(2).is<picojson::null>());
}
