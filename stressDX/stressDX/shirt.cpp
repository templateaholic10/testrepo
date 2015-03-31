#include "shirt.h"

namespace psychotest {

	SHIRT::SHIRT()
		: _enabled(true), _gr(nullptr)
	{
		_ferr.open(ERRPATH + "shirt.err");
	}

	SHIRT::~SHIRT()
	{
		if (_ferr.is_open()) {
			_ferr.close();
		}
	}

	int SHIRT::setGraphics(layout::GRAPHICS &gr)
	{
		_gr = &gr;

		return MYSUCCESS;
	}

	int SHIRT::initialize()
	{
		_timer = 0;
		_enabled = true;

		return MYSUCCESS;
	}

	int SHIRT::process()
	{
		_enabled = (_timer == 0);
		return MYSUCCESS;
	}

	int SHIRT::overlay()
	{
		if (_enabled) {
			if (DrawExtendGraph(_left, _top, _left + _width + 1, _top + _height + 1, _gr->_handle, TRUE) != 0) {
				return MYERROR;
			}
		}
		return MYSUCCESS;
	}

	int SHIRT::step()
	{
		if (_timer == 1) {
			_enabled = true;
			_timer--;
		} else if (_timer > 0) {
			_timer--;
		}
		return MYSUCCESS;
	}

	bool SHIRT::_Enabled()
	{
		return _enabled;
	}

}
