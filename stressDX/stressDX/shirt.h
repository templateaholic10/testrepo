#ifndef SHIRT_H
#define SHIRT_H

#include "layout.h"
#include "graphics.h"

namespace psychotest {

	class SHIRT;

	class SHIRT : public layout::COMPONENT {
	public:
		SHIRT();
		~SHIRT();

		int setGraphics(layout::GRAPHICS &gr);

		int initialize();
		int process();
		int overlay();
		int step();

	public:
		bool _Enabled();

	private:
		// コンポーネントの描画関係
		bool _enabled;
		layout::GRAPHICS *_gr;

		// エラー出力用ファイル
		std::ofstream _ferr;
	};

}

#endif