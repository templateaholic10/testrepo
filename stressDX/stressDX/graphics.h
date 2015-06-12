#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <DxLib.h>
#include "commons.h"

namespace layout {

	class GRAPHICS;

	class GRAPHICS {
	public:
		GRAPHICS();
		~GRAPHICS();
		int getHandle(const std::string name);

	public:
		int _handle;
		int _width;
		int _height;
	};

}

#endif