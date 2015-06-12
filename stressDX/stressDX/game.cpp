#include "game.h"

namespace psychotest {

	int GAME::_width(0);
	int GAME::_height(0);
	int GAME::_colorBit(0);
	int GAME::_colorBitDepth(0);

	std::string GAME::_name("");

	SHIRT GAME::_shirtL;
	SHIRT GAME::_shirtR;

	int GAME::_graphnum(0);
	layout::GRAPHICS *GAME::_gr(nullptr);
	int GAME::_testnum(0);
	int *GAME::_left(nullptr);
	int *GAME::_right(nullptr);

	int GAME::_now(0);
	int GAME::_interval(0);

	int GAME::_mouseClick(0);

	std::ofstream GAME::_ferr(ERRPATH + "game.err");
	std::ofstream GAME::_fout;

	GAME::GAME()
	{
	}

	GAME::~GAME()
	{
	}

	int GAME::setWindow()
	{
		_colorBit = 16;
		if (GetScreenState(&_width, &_height, &_colorBitDepth) == -1) {
			_width = 0;
			_height = 0;
			_colorBitDepth = 0;
			_ferr << "error : setWindow（GetScreenStateできません）" << std::endl;
			return MYERROR;
		}
		return MYSUCCESS;
	}

	int GAME::inputTest()
	{
		std::string line, word;
		std::ifstream fin(STRUCTPATH + _name + ".input");
		if (fin.fail()) {
			return MYERROR;
		}

		getline(fin, line); // タイトル行

		fin >> _graphnum; getline(fin, line);
		_gr = new layout::GRAPHICS[_graphnum];

		for (int i = 0; i < _graphnum; i++)
		{
			fin >> word; getline(fin, line);
			if (_gr[i].getHandle(word) == MYERROR) {
				fin.close();
				return MYERROR;
			}
		}

		getline(fin, line); // 空行

		getline(fin, line); // タイトル行

		fin >> _testnum; getline(fin, line);
		_left = new int[_testnum];
		_right = new int[_testnum];
		for (int i = 0; i < _testnum; i++)
		{
			fin >> _left[i] >> _right[i]; getline(fin, line);
		}

		fin.close();

		return MYSUCCESS;
	}

	int GAME::inputCfg()
	{
		std::string line;
		double left, right, top, bottom, middle;
		std::ifstream fin(STRUCTPATH + _name + ".cfg");
		if (fin.fail()) {
			return MYERROR;
		}

		getline(fin, line); // タイトル行

		fin >> left; getline(fin, line);
		fin >> right; getline(fin, line);
		fin >> top; getline(fin, line);
		fin >> bottom; getline(fin, line);
		fin >> middle; getline(fin, line);

		_shirtL.setSize(_width, _height, left, 0.5 + middle / 2, top, bottom);
		_shirtR.setSize(_width, _height, 0.5 + middle / 2, right, top, bottom);

		getline(fin, line); // 空行

		getline(fin, line); // タイトル行

		fin >> _interval; getline(fin, line);

		fin.close();

		return MYSUCCESS;
	}

	int GAME::nextEvent()
	{
		int old = _mouseClick;
		_mouseClick = GetMouseInput();

		if (_shirtL._Enabled() && _shirtR._Enabled()) {
			if ((((_mouseClick & MOUSE_INPUT_LEFT) != 0)
				&& ((old & MOUSE_INPUT_LEFT) == 0))
				|| (((_mouseClick & MOUSE_INPUT_RIGHT) != 0)
				&& ((old & MOUSE_INPUT_RIGHT) == 0))) {
				// 左または右ボタンが押下された瞬間
				_fout << "STEP " << std::setw(3) << _now << " : " << (((_mouseClick & MOUSE_INPUT_LEFT) != 0) ? 'L' : 'R') << std::endl;
				_now++;
				if (_now >= _testnum) {
					// _ferr << "terminated" << std::endl;
					_fout.close();
					return MYTERMINATE;
				}
				_shirtL.setGraphics(_gr[_left[_now]]);
				_shirtR.setGraphics(_gr[_right[_now]]);
				_shirtL.setTimer(_interval);
				_shirtR.setTimer(_interval);
				// _ferr << "STEP : " << _now << std::endl;
			}
		}

		return MYSUCCESS;
	}

	int GAME::input()
	{
		if (inputCfg() == MYERROR) {
			return MYERROR;
		}
		if (inputTest() == MYERROR) {
			return MYERROR;
		}
		return MYSUCCESS;
	}

	int GAME::initialize()
	{
		if (_shirtL.initialize() == MYERROR) {
			return MYERROR;
		}
		if (_shirtR.initialize() == MYERROR) {
			return MYERROR;
		}

		_shirtL.setGraphics(_gr[_left[0]]);
		_shirtR.setGraphics(_gr[_right[0]]);
		_now = 0;
		_fout.open(OUTPUTPATH + _name + ".output");

		return MYSUCCESS;
	}

	int GAME::process()
	{
		int tmp;

		if ((tmp = nextEvent()) == MYERROR) {
			return MYERROR;
		} else if (tmp == MYTERMINATE) {
			return MYTERMINATE;
		}
		if (_shirtL.process() == MYERROR) {
			return MYERROR;
		}
		if (_shirtR.process() == MYERROR) {
			return MYERROR;
		}

		return MYSUCCESS;
	}

	int GAME::overlay()
	{
		if (_shirtL.overlay() == MYERROR) {
			return MYERROR;
		}
		if (_shirtR.overlay() == MYERROR) {
			return MYERROR;
		}

		return MYSUCCESS;
	}

	int GAME::step()
	{
		if (_shirtL.step() == MYERROR) {
			return MYERROR;
		}
		if (_shirtR.step() == MYERROR) {
			return MYERROR;
		}
		return MYSUCCESS;
	}

	int GAME::terminate()
	{
		_ferr.close();
		return MYSUCCESS;
	}


}