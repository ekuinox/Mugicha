#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include <time.h>
#include <windows.h>
#include "game.h"

#if 1
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#endif

#define unless(expression) if(!expression)

/* Constant */
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "É|ÉGÅ["
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

/* Prototype Declaration */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE, HWND, BOOL);
void Uninit(void);