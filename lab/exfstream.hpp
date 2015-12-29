/*! @file
    @brief fstreamのユーティリティ関数
    @auther yanteyon10
    @date 11/13
*/

#ifndef EXFSTREAM_HPP
#define EXFSTREAM_HPP

#include <cassert>
#include <fstream>

// 名前空間に入れると，その中の演算子しか探索しない

/*! @brief ofstreamのラッパー関数
*/
template <class T>
bool dump(const std::string& filename, const T& obj, std::ios_base::openmode mode = std::ios_base::out)
{
    std::ofstream fout(filename, mode);
    if (fout.fail()) {
        assert(false);
        return false;
    }
    fout << obj << std::endl;
    fout.close();
    return true;
}

/*! @brief ifstreamのラッパー関数
*/
template <class T>
bool load(const std::string& filename, T& obj, std::ios_base::openmode mode = std::ios_base::in)
{
    std::ifstream fin(filename, mode);
    if (fin.fail()) {
        assert(false);
        return false;
    }
    fin >> obj;
    fin.close();
    return true;
}

#endif
