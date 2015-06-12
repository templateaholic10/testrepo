#include "game.h"

int inputStart(int &displayWidth, int &displayHeight, int &windowWidth, int &windowHeight, int &windowMode, std::string &name)
{
	std::string line, word;
	std::ifstream fin(psychotest::STRUCTPATH + "start.ini");
	if (fin.fail()) {
		return MYERROR;
	}

	getline(fin, line); // �^�C�g���s

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

// �v���O������ WinMain ����n�܂�܂�
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

	// DxLib_Init()�O�ɏ�����ʃ��[�h��ݒ�

	// �E�B���h�E���[�h�ύX
	if (ChangeWindowMode(windowMode) != 0) {
		return MYERROR;
	}
	// �t���X�N���[�����[�h�̏ꍇ�C������ʂ̉𑜓x��ύX
	// �������O�͕K����������̂ŕԂ�l�̓`�F�b�N���Ȃ�
	if (windowMode == TRUE) {
		SetGraphMode(windowWidth, windowHeight, colorBit);
	} else {
		SetGraphMode(displayWidth, displayHeight, colorBit);
	}
	// �}�E�X�J�[�\����\��
	if (SetMouseDispFlag(TRUE) != 0) {
		return MYERROR;
	}

	// �������Ɨ���ʐݒ�
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

	// while(����ʂ�\��ʂɔ��f, ���b�Z�[�W����, ��ʃN���A)
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

	DxLib_End(); // DX���C�u�����I������

	return MYSUCCESS;
}