#include "debug_console.h"

#include <Windows.h>
#include <iostream>

DebugConsole::DebugConsole() :
	out(nullptr),
	in(nullptr){
	init();
}

// デストラクタ
DebugConsole::~DebugConsole() {
	std::cout << "DebugConsole Exit." << std::endl;
	uninit();
}

// 初期化
bool DebugConsole::init() {

	if (AllocConsole()) {
		// 成功
	}
	else {
		return false;
	}

	// 作成したコンソールを標準出力に設定
	freopen_s(&out, "CONOUT$", "w", stdout);	//　出力をコンソールに設定
	freopen_s(&in, "CONIN$", "r", stdin);		//　出力をコンソールに設定

	std::cout << "DebugConsole Initialize." << std::endl;

	return true;
}

// 終了処理
void DebugConsole::uninit() {
	fclose(out);
	fclose(in);
	FreeConsole();
}

// コンソールウィンドウの作成
void DebugConsole::create_console_window() {
	static DebugConsole instance;
}

// Initメッセージ出力
void DebugConsole::message(const char* str) {
	std::cout << "Initilize" << str << "." << std::endl;
}