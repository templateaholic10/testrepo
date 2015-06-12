#include "layout.h"

namespace layout {

	COMPONENT::COMPONENT()
		: _left(0), _right(0), _top(0), _bottom(0)
	{
	}

	COMPONENT::COMPONENT(int left, int right, int top, int bottom, int width, int height)
		: _left(left), _right(right), _top(top), _bottom(bottom), _width(width), _height(height), _timer(0)
	{
	}

	COMPONENT::COMPONENT(int winWidth, int winHeight, double left, double right, double top, double bottom)
		: _left((int)(winWidth * left)), _right((int)(winWidth * right)), _top((int)(winHeight * top)), _bottom((int)(winHeight * bottom)), _width(winWidth - _left - _right), _height(winHeight - _top - _bottom)
	{
	}

	COMPONENT::~COMPONENT()
	{
	}

	int COMPONENT::setSize(int left, int right, int top, int bottom, int width, int height)
	{
		_left = left;
		_right = right;
		_top = top;
		_bottom = bottom;
		_width = width;
		_height = height;

		return MYSUCCESS;
	}

	int COMPONENT::setSize(int winWidth, int winHeight, double left, double right, double top, double bottom)
	{
		_left = (int)(winWidth * left);
		_right = (int)(winWidth * right);
		_top = (int)(winHeight * top);
		_bottom = (int)(winHeight * bottom);
		_width = winWidth - _left - _right;
		_height = winHeight - _top - _bottom;

		return MYSUCCESS;
	}

	int COMPONENT::setTimer(int timer)
	{
		_timer = timer;

		return MYSUCCESS;
	}

}