#include "debug_console.h"

#include <Windows.h>
#include <iostream>

DebugConsole::DebugConsole() :
	out(nullptr),
	in(nullptr){
	init();
}

// �f�X�g���N�^
DebugConsole::~DebugConsole() {
	std::cout << "DebugConsole Exit." << std::endl;
	uninit();
}

// ������
bool DebugConsole::init() {

	if (AllocConsole()) {
		// ����
	}
	else {
		return false;
	}

	// �쐬�����R���\�[����W���o�͂ɐݒ�
	freopen_s(&out, "CONOUT$", "w", stdout);	//�@�o�͂��R���\�[���ɐݒ�
	freopen_s(&in, "CONIN$", "r", stdin);		//�@�o�͂��R���\�[���ɐݒ�

	std::cout << "DebugConsole Initialize." << std::endl;

	return true;
}

// �I������
void DebugConsole::uninit() {
	fclose(out);
	fclose(in);
	FreeConsole();
}

// �R���\�[���E�B���h�E�̍쐬
void DebugConsole::create_console_window() {
	static DebugConsole instance;
}

// Init���b�Z�[�W�o��
void DebugConsole::message(const char* str) {
	std::cout << "Initilize" << str << "." << std::endl;
}