#include "graphics.h"

namespace layout {

	GRAPHICS::GRAPHICS()
	{
	}

	GRAPHICS::~GRAPHICS()
	{
	}

	int GRAPHICS::getHandle(const std::string name)
	{
		if ((_handle = LoadGraph((psychotest::FIGPATH + name).c_str())) == -1) {
			return MYERROR; // 画像ロードエラー
		}
		if (GetGraphSize(_handle, &_width, &_height) == -1) {
			return MYERROR;
		}
		return MYSUCCESS;
	}

}