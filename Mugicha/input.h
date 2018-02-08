#ifndef _INPUT_H_
#define _INPUT_H_

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#include <dinput.h>

HRESULT InitInput(HINSTANCE, HWND);
void UninitInput(void);
void UpdateInput(void);

HRESULT InitKeyboard(HINSTANCE, HWND);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int);
bool GetKeyboardTrigger(int);
bool GetKeyboardRepeat(int);
bool GetKeyboardRelease(int);

#endif