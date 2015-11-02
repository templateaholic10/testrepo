/*! @file
    @brief タイマークラス
    @auther yanteyon10
    @date 11/2
*/

#include <chrono>

/*! @class
    @brief タイマークラス．定義時に計測開始し，elapsedをコールすると経過時間を返す．restartでリセットすることもできる．
    @tparam Unit 時間の単位．小数部分は測定されない点に注意
*/
template <typename Unit = std::chrono::milliseconds>
class Timer {
    using unit_type = Unit;
private:
    std::chrono::system_clock::time_point start;
public:
    Timer()
    {
        restart();
    }
    void restart()
    {
        start = std::chrono::system_clock::now();
    }
    int elapsed()
    {
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        return std::chrono::duration_cast<Unit>(end - start).count();
    }
};
