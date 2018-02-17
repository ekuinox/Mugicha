#include "main.h"

/* global variable */
LPDIRECT3D9 d3d = NULL; // Direct3D object
LPDIRECT3DDEVICE9 d3d_device = NULL; // Devece obj
HWND hWnd;

/* main */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&wcex);

	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL
	);
#ifdef _DEBUG
	DebugConsole::create_console_window();
	std::cout << "ハロー　ハロー" << std::endl;
#endif
	if (FAILED(Init(hInstance, hWnd, TRUE))) return -1;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* message loop */
	auto controller = new Controller();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				// when PostQuitMessage() called
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			controller->exec();
		}
	}
	delete controller;

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Uninit();

	return static_cast<int>(msg.wParam);
}

/* window procedure */
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_DESTROY == uMsg)
	{
		PostQuitMessage(0);
		return S_OK;
	}
	if (WM_CLOSE == uMsg)
	{
		if (MessageBox(hWnd, "終了しますか?", "", MB_YESNO) == IDYES)
		{
			DestroyWindow(hWnd);
		}
		return S_OK;
	}
	if (WM_KEYDOWN == uMsg)
	{
		if (VK_ESCAPE == wParam)
		{
			// 確認なしで終了させる
			DestroyWindow(hWnd);
			/*
			if (MessageBox(hWnd, "終了しますか?", "", MB_YESNO) == IDYES)
			{
			DestroyWindow(hWnd);
			}
			*/
			return S_OK;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/* initialize */
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) return E_FAIL;

	if (FAILED(d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) return E_FAIL;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = bWindow;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (bWindow)
	{
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3d_device)))
	{
		if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3d_device)))
		{
			if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3d_device)))
			{
				MessageBox(hWnd, "error", "", 0);
				return E_FAIL;
			}
		}

	}

	d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	srand(static_cast<unsigned>(time(NULL)));

	// keyboard
	InitKeyboard(hInstance, hWnd);

	// xaudio2
	//	InitSound();

	return S_OK;
}

/* close */
void Uninit(void)
{
	if (d3d_device)
	{
		d3d_device->Release();
		d3d_device = NULL;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = NULL;
	}

	UninitKeyboard();

	/* XAUDIO */
	//	UninitSound();
}