#ifndef GAME_H
#define GAME_H

#include "shirt.h"

namespace psychotest {

	const int MYTERMINATE = 10;

	class GAME;

	class GAME {
	public:
		~GAME();
		static int setWindow();
		static int inputTest();
		static int inputCfg();
		static int nextEvent();

		static int input();
		static int initialize();
		static int process();
		static int overlay();
		static int step();
		static int terminate();

	public:
		static int _width;
		static int _height;
		static int _colorBit;
		static int _colorBitDepth;

		static std::string _name;

		static SHIRT _shirtL, _shirtR;

		static int _graphnum;
		static layout::GRAPHICS *_gr;
		static int _testnum;
		static int *_left, *_right;

		static int _now;
		static int _interval;

		static int _mouseClick;

	private:
		GAME(); // インスタンスを作らない

		static std::ofstream _ferr;
		static std::ofstream _fout;
	};

}

#endif