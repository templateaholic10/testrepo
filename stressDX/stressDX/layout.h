#ifndef LAYOUT_H
#define LAYOUT_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include "commons.h"

namespace layout {

	class COMPONENT;

	class COMPONENT {
	public:
		COMPONENT();
		COMPONENT(int left, int right, int top, int bottom, int width, int height);
		COMPONENT(int winWidth, int winHeight, double left, double right, double top, double bottom);
		~COMPONENT();

		int setSize(int left, int right, int top, int bottom, int width, int height);
		int setSize(int winWidth, int winHeight, double left, double right, double top, double bottom);
		int setTimer(int timer);

		virtual int initialize() = 0;
		virtual int process() = 0;
		virtual int overlay() = 0;
		virtual int step() = 0;

	public:
		int _left, _right, _top, _bottom;
		int _width, _height;
		int _timer;

	private:
	};

}

#endif