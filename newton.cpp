#include <iostream>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

double burgers(double v, double x, double t)
{
    if (t == 0) {
        cerr << "0 divided." << endl;
        exit(1);
    }

    return sin(v - x) - v / t;
}

double dburgers(double v, double x, double t)
{
    if (t == 0) {
        cerr << "0 divided." << endl;
        exit(1);
    }

    return cos(v - x) - 1 / t;
}

class newton
{
public:
    newton();
    ~newton();

    void   init(double x, double t, double start);
    double solve();

private:
    double          _x;
    double          _t;
    vector <double> _pass;
    int             _step;
    const double    _epsilon = 0.001;

    bool step_forward();
};

newton::newton()
{
}

newton::~newton()
{
}

bool newton::step_forward()
{
    _step++;
    double last = _pass.back();
    if (dburgers(last, _x, _t) == 0) {
        cerr << "0 divided." << endl;

        return false;
    }
    _pass.push_back(last - burgers(last, _x, _t) / dburgers(last, _x, _t));
    clog << "x" << _x << " " << _step << ": " << _pass.back() << endl;

    if (abs(burgers(_pass.back(), _x, _t) - burgers(last, _x, _t)) < _epsilon) {
        return true;
    } else {
        return false;
    }
}

void newton::init(double x, double t, double start)
{
    _x = x;
    _t = t;
    _pass.clear();
    _pass.push_back(start);
    _step = 0;
}

double newton::solve()
{
    int                          count = 0;
    random_device                rnd;
    mt19937                      mt(rnd());
    uniform_real_distribution <> xrand(-M_PI, M_PI);

    while (!step_forward()) {
        if (count > 10000) {
            count = 0;
            _pass.clear();
            double start = xrand(mt);
            _pass.push_back(start);
            _step = 0;
        }
        count++;
    }

    return _pass.back();
}

int main(int argc, char const *argv[])
{
    double    t[]  = { 1.0 / 2, 1.0, 2.0 };
    const int TLEN = 3;

    const int NUM = 100;
    double    x[NUM + 1];
    newton    N;

    const int flag    = 1;
    double    diviser = 1 / t[flag];
    double    tmp;

    for (int j = 0; j <= NUM; ++j)
    {
        x[j] = (j * 2.0 - NUM) / NUM * M_PI;
    }

    for (int j = 0; j <= NUM; ++j)
    {
        N.init(x[j], t[flag], 0);
        tmp = N.solve() * diviser;
        cout << x[j] << "," << tmp << endl;
    }

    /*
        for (int j = 0; j <= 0.61 * NUM; ++j)
        {
            N.init(x[j], t[flag], M_PI);
            tmp = N.solve() * diviser;
            cout << x[j] << "," << tmp << endl;
        }

        for (int j = 0.39 * NUM; j <= 0.61 * NUM; ++j)
        {
            N.init(x[j], t[flag], 0);
            tmp = N.solve() * diviser;
            cout << x[j] << "," << tmp << endl;
        }

        for (int j = 0.39 * NUM; j <= NUM; ++j)
        {
            N.init(x[j], t[flag], -M_PI);
            tmp = N.solve() * diviser;
            cout << x[j] << "," << tmp << endl;
        }
    */
    return 0;
}
