#pragma once
#ifdef _DEBUG

#include <cstdio>

class DebugConsole {
	FILE *out;
	FILE *in;

	DebugConsole();
	bool init();
	void uninit();
public:
	~DebugConsole();
	static void create_console_window();
	static void message(const char* str);
};

#endif