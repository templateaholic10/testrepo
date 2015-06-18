#include <iostream>
#include <string>
using namespace std;

class game {
public:
    game ();
    virtual ~game ();

    enum RESOURCE {BRICK, LUMBER, WOOL, GRAIN, ORE};
    static int const RESOURCE_NUM = 5;
    enum DEVELOPMENT {KNIGHT, PLENTY, ROAD_BUILD, MONOPOLY, VP};
    static int const DEVELOPMENT_NUM = 5;
    static int const TILE_NUM = 19;

private:
    string name;
    int _map[TILE_NUM];
};

game::game () 
{

}

game::~game () 
{

}

class player {
public:
    player ();
    virtual ~player ();

private:
    string name;
    int _resource[game::RESOURCE_NUM];
    int _development[game::DEVELOPMENT_NUM];
};

player::player ()
{

}

player::~player ()
{

}

int main(int argc, char const *argv[]) {
    cout << "hello, yey!" << endl;
    return 0;
}
