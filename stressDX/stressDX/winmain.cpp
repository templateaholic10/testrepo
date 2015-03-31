#include "game.h"

int inputStart(int &displayWidth, int &displayHeight, int &windowWidth, int &windowHeight, int &windowMode, std::string &name)
{
	std::string line, word;
	std::ifstream fin(psychotest::STRUCTPATH + "start.ini");
	if (fin.fail()) {
		return MYERROR;
	}

	getline(fin, line); // タイトル行

	fin >> displayWidth; getline(fin, line);
	fin >> displayHeight; getline(fin, line);
	fin >> windowWidth; getline(fin, line);
	fin >> windowHeight; getline(fin, line);
	fin >> word; getline(fin, line);
	if (word == "no") {
		windowMode = FALSE;
	}
	else {
		windowMode = TRUE;
	}
	fin >> name; getline(fin, line);

	fin.close();

	return MYSUCCESS;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int displayWidth;
	int displayHeight;
	int windowWidth;
	int windowHeight;
	int windowMode;
	const int colorBit = 16;

	if (inputStart(displayWidth, displayHeight, windowWidth, windowHeight, windowMode, psychotest::GAME::_name) == MYERROR) {
		return MYERROR;
	}

	// DxLib_Init()前に初期画面モードを設定

	// ウィンドウモード変更
	if (ChangeWindowMode(windowMode) != 0) {
		return MYERROR;
	}
	// フルスクリーンモードの場合，初期画面の解像度を変更
	// 初期化前は必ず成功するので返り値はチェックしない
	if (windowMode == TRUE) {
		SetGraphMode(windowWidth, windowHeight, colorBit);
	} else {
		SetGraphMode(displayWidth, displayHeight, colorBit);
	}
	// マウスカーソルを表示
	if (SetMouseDispFlag(TRUE) != 0) {
		return MYERROR;
	}

	// 初期化と裏画面設定
	if (DxLib_Init() != 0 || SetDrawScreen(DX_SCREEN_BACK) != 0) {
		return MYERROR;
	}

	if (psychotest::GAME::setWindow() == MYERROR) {
		return MYERROR;
	}
	if (psychotest::GAME::input() == MYERROR) {
		return MYERROR;
	}
	if (psychotest::GAME::initialize() == MYERROR) {
		return MYERROR;
	}

	// while(裏画面を表画面に反映, メッセージ処理, 画面クリア)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0){
		
		int tmp;

		if ((tmp = psychotest::GAME::process()) == MYERROR) {
			return MYERROR;
		} else if (tmp == psychotest::MYTERMINATE) {
			break;
		}
		if (psychotest::GAME::overlay() == MYERROR) {
			return MYERROR;
		}
		if (psychotest::GAME::step() == MYERROR) {
			return MYERROR;
		}
		
	}

	if (psychotest::GAME::terminate() == MYERROR) {
		DxLib_End();
		return MYERROR;
	}

	DxLib_End(); // DXライブラリ終了処理

	return MYSUCCESS;
}