#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
using namespace std;

double burgers(double v, double x, double t)
{
    return sin(v - x) - v / t;
}

double dburgers(double v, double x, double t)
{
    return cos(v - x) - 1 / t;
}

class newton
{
public:
    newton(double(*f)(double), double(*df)(double), double start);
    ~newton();

    double solve();

private:
    double (*_f)(double);
    double    (*_df)(double);
    vector <double> _pass;
    int             _step;
    double          _epsilon;

    bool step_forward();
};

newton::newton(double(*f)(double), double(*df)(double), double start)
{
    _f  = f;
    _df = df;
    _pass.push_back(start);
    _step = 0;
}

bool newton::step_forward()
{
    double last = _pass.back();
    _pass.push_back(last - _f(last) / _df(last));
    _step++;

    if (abs(_f(_pass.back()) - _f(last)) < _epsilon) {
        return true;
    } else {
        return false;
    }
}

double newton::solve()
{
    while (!step_forward()) { }

    return _pass.back();
}

int main(int argc, char const *argv[])
{
    double    t[]  = { 0, 1 / 2, 1, 2 };
    const int TLEN = 4;

    const int    NUM    = 100;
    const double OFFSET = 2 * M_PI / NUM;
    double       x      = -M_PI;
    double       v[TLEN][NUM + 1];

    for (int i = 0; i < 1; ++i) // todo
    {
        for (int j = 0; j <= 0; ++j) // todo
        {
            auto const f  = bind(burgers, placeholders::_1, x, t[i]);
            auto const df = bind(dburgers, placeholders::_1, x, t[i]);

            newton N(f, df, 0);
            v[i][j] = N.solve();

            x += OFFSET;
        }
    }

    return 0;
}
