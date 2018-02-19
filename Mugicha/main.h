#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include <time.h>
#include <windows.h>
#include "conf.h"
#include "controller.h"

#if 1
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#endif

#ifdef _DEBUG
#include "debug_console.h"
#endif

/* Prototype Declaration */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE, HWND, BOOL);
void Uninit(void);