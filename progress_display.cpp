char *gets(char *str);
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/progress.hpp>

int main(int argc, char const *argv[]) {

    boost::progress_display progress(100);
    for(int i=0; i<100; i++){
        ++progress;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
