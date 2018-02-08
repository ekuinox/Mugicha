#include "input.h"

#define NUM_KEY_MAX (256)
#define LIMIT_COUNT_REPEAT (20)

LPDIRECTINPUT8 g_pDInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDIDevKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pDIDevMouse = NULL;

BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];
BYTE g_aKeyStateRelease[NUM_KEY_MAX];
int g_aKeyStateRepeatCnt[NUM_KEY_MAX];

/* 入力処理の初期化 */
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	if (g_pDInput == NULL)
	{
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL);
		return hr;
	}
	return S_OK;
}

/* 入力処理の更新処理 */
void UpdateInput()
{
	UpdateKeyboard();
}

/* 入力処理の終了処理 */
void UninitInput()
{
	if (g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

/* キーボードの初期化 */
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	hr = InitInput(hInstance, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れない．", "X(", MB_ICONWARNING);
		return hr;
	}

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードが見つからない．", "X(", MB_ICONWARNING);
		return hr;
	}
	
	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータのフォーマットに失敗．", "X(", MB_ICONWARNING);
		return hr;
	}

	// 強調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの強調モードを設定できなかった．", "X(", MB_ICONWARNING);
		return hr;
	}
	
	g_pDIDevKeyboard->Acquire();
	
	return S_OK;
}

/* キーボードの更新処理 */
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	if (SUCCEEDED(g_pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キートリガー・リリース情報を生成．
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & ~aKeyState[nCnKey];

			// キーピート情報を生成
			if (aKeyState[nCnKey])
			{
				if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if (g_aKeyStateRepeatCnt[nCnKey] == 1
						|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
					else
					{
						g_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCnKey] = 0;
				g_aKeyStateRepeat[nCnKey] = 0;
			}

			// キープレス情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDIDevKeyboard->Acquire();
	}
}

/* キーボードの終了処理 */
void UninitKeyboard(void)
{
	if (g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Unacquire();
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}

	UninitInput();
}

/* キーボードのプレス状態を取得 */
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

/* キーボードのトリガ状態を取得 */
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

/* キーボードのリピート状態を取得 */
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

/* キーボードのリリース状態を取得 */
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}