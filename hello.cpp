#include <iostream>
#include <string>
using namespace std;

class game {
public:
    game ();
    virtual ~game ();

    enum RESOURCE {BRICK, LUMBER, WOOL, GRAIN, ORE};
    const int RESOURCE_NUM = 5;
    enum DEVELOPMENT {KNIGHT, PLENTY, ROAD_BUILD, MONOPOLY, VP};
    const int DEVELOPMENT_NUM = 5;

private:
    string name;
    const int TILE_NUM = 19;
    _map[TILE_NUM];
};

class player {
public:
    player ();
    virtual ~player ();

private:
    string name;
    int _resource[RESOURCE_NUM];
    int _development[DEVELOPMENT_NUM];
};

int main(int argc, char const *argv[]) {
    cout << "hello, yey177!" << endl;
    return 0;
}
