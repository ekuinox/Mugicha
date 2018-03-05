#include "XInputController.h"

//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define MAX_CONTROLLERS	4	// 最大コントローラー数(XInputが認識できるのは4つまで)
#define FNC_XINPUTGETSTATEEX	((const char*)100)	// XInputGetStateEX用

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct CONTROLLER_STATE{
	XINPUT_STATE State;						// 現在の入力情報
	XINPUT_STATE Old;						// 前回の入力情報
	XINPUT_VIBRATION Vibration;				// バイブレーション情報
	XINPUT_BATTERY_INFORMATION Battery;		// バッテリー情報
	bool Connected;							// 接続確認情報
};

//*****************************************************************************
// グローバル
//*****************************************************************************
CONTROLLER_STATE Controller[MAX_CONTROLLERS];	// コントローラー構造体

BYTE	LEFT_TRIGGER_DEADZONE	= 255 / 10;		// 左トリガーのしきい値
BYTE	RIGHT_TRIGGER_DEADZONE	= 255 / 10;		// 右トリガーのしきい値
USHORT	LEFT_STICK_DEADZONE		= 32767 / 3;	// 左スティックのしきい値
USHORT	RIGHT_STICK_DEADZONE	= 32767 / 3;	// 右スティックのしきい値

/* XInputDLL */
using XINPUTGETSTATEEX = DWORD(WINAPI *)(DWORD dwUserIndex, XINPUT_STATE *pState);
XINPUTGETSTATEEX XInputGetStateEx = nullptr;
HMODULE XInput_dll = nullptr;

//=============================================================================
// XInputDLLのロード
//=============================================================================
void LoadXInputDLL(void) {
	XInput_dll = ::LoadLibrary("xinput1_4.dll");
	if (XInput_dll == NULL) {
		XInput_dll = ::LoadLibrary("xinput1_3.dll");
	}
	else {
		XInputGetStateEx = (XINPUTGETSTATEEX)::GetProcAddress(XInput_dll, FNC_XINPUTGETSTATEEX);
	}
}

//=============================================================================
// XInputDLLの解放
//=============================================================================
void FreeXInputDLL(void) {
	if (XInput_dll != NULL) {
		::FreeLibrary(XInput_dll);
	}
}

//=============================================================================
// コントローラー更新処理
//=============================================================================
HRESULT UpdateController(void) {
	DWORD Result;

	for (BYTE i = 0; i < MAX_CONTROLLERS; i++) {
		SecureZeroMemory(&Controller[i].Old, sizeof(XINPUT_STATE));						// 前回の入力情報のメモリ初期化
		Controller[i].Old = Controller[i].State;										// 現在の入力情報を前回の入力情報へ保存
		SecureZeroMemory(&Controller[i].State, sizeof(XINPUT_STATE));					// 現在の入力情報のメモリ初期化
		SecureZeroMemory(&Controller[i].Battery, sizeof(XINPUT_BATTERY_INFORMATION));	// バッテリー情報のメモリ初期化

		/* コントローラー情報取得 */
		if (XInput_dll == NULL) {									// XInputDLLが無い場合
			Result = XInputGetState(i, &Controller[i].State);		// 入力情報取得(ガイドボタンなし)
		}
		else {														// XInputDLLが有る場合
			Result = XInputGetStateEx(i, &Controller[i].State);		// 入力情報取得(ガイドボタンあり)
		}
		XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &Controller[i].Battery);	// バッテリー情報取得

		/* 接続確認 */
		if (Result == ERROR_SUCCESS) {
			Controller[i].Connected = true;		// 接続されている
		}
		else {
			Controller[i].Connected = false;	// 接続されていない
		}
	}

	return S_OK;
}

//=============================================================================
// ボタンのプレス状態を取得
//=============================================================================
bool GetControllerButtonPress(WORD Btn, BYTE cNum) {
	if (Controller[cNum].Connected) {
		return (Controller[cNum].State.Gamepad.wButtons & Btn) != 0;
	}
	return 0;
}

//=============================================================================
// ボタンのトリガー状態を取得
//=============================================================================
bool GetControllerButtonTrigger(WORD Btn, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if ((Controller[cNum].State.Gamepad.wButtons & Btn) != (Controller[cNum].Old.Gamepad.wButtons & Btn)) {
			return (Controller[cNum].State.Gamepad.wButtons & Btn) != 0;
		}
	}
	return 0;
}

//=============================================================================
// ボタンのリリース状態を取得
//=============================================================================
bool GetControllerButtonRelease(WORD Btn, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if ((Controller[cNum].State.Gamepad.wButtons & Btn) != (Controller[cNum].Old.Gamepad.wButtons & Btn)) {
			return (Controller[cNum].Old.Gamepad.wButtons & Btn) != 0;
		}
	}
	return 0;
}

//=============================================================================
// トリガーのプレス状態を取得
//=============================================================================
BYTE GetControllerTriggerPress(LPCSTR LR, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if (strcmp(LR, XIP_LEFT) == 0) {
			return (Controller[cNum].State.Gamepad.bLeftTrigger > LEFT_TRIGGER_DEADZONE);
		}
		else if (strcmp(LR, XIP_RIGHT) == 0) {
			return (Controller[cNum].State.Gamepad.bRightTrigger > RIGHT_TRIGGER_DEADZONE);
		}
	}
	return 0;
}

//=============================================================================
// トリガーのトリガー状態を取得
//=============================================================================
BYTE GetControllerTriggerTrigger(LPCSTR LR, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if (strcmp(LR, XIP_LEFT) == 0) {
			if ((Controller[cNum].State.Gamepad.bLeftTrigger > LEFT_TRIGGER_DEADZONE) != (Controller[cNum].Old.Gamepad.bLeftTrigger > LEFT_TRIGGER_DEADZONE)) {
				return (Controller[cNum].State.Gamepad.bLeftTrigger > LEFT_TRIGGER_DEADZONE);
			}
		}
		else if (strcmp(LR, XIP_RIGHT) == 0) {
			if ((Controller[cNum].State.Gamepad.bRightTrigger > RIGHT_TRIGGER_DEADZONE) != (Controller[cNum].Old.Gamepad.bRightTrigger > RIGHT_TRIGGER_DEADZONE)) {
				return (Controller[cNum].State.Gamepad.bRightTrigger > RIGHT_TRIGGER_DEADZONE);
			}
		}
	}
	return 0;
}

//=============================================================================
// トリガーのリリース状態を取得
//=============================================================================
BYTE GetControllerTriggerRelease(LPCSTR LR, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if (strcmp(LR, XIP_LEFT) == 0) {
			if ((Controller[cNum].State.Gamepad.bLeftTrigger > LEFT_TRIGGER_DEADZONE) != (Controller[cNum].Old.Gamepad.bLeftTrigger > LEFT_TRIGGER_DEADZONE)) {
				return (Controller[cNum].Old.Gamepad.bLeftTrigger > LEFT_TRIGGER_DEADZONE);
			}
		}
		else if (strcmp(LR, XIP_RIGHT) == 0) {
			if ((Controller[cNum].State.Gamepad.bRightTrigger > RIGHT_TRIGGER_DEADZONE) != (Controller[cNum].Old.Gamepad.bRightTrigger > RIGHT_TRIGGER_DEADZONE)) {
				return (Controller[cNum].Old.Gamepad.bRightTrigger > RIGHT_TRIGGER_DEADZONE);
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックのプレス状態を取得
//=============================================================================
SHORT GetControllerStickPress(LPCSTR LR, LPCSTR UDLR, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if (strcmp(LR, XIP_LEFT) == 0) {
			if (strcmp(UDLR, XIP_S_UP) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbLY > LEFT_STICK_DEADZONE);
			}
			else if (strcmp(UDLR, XIP_S_DOWN) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE);
			}
			else if (strcmp(UDLR, XIP_S_LEFT) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE);
			}
			else if (strcmp(UDLR, XIP_S_RIGHT) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbLX > LEFT_STICK_DEADZONE);
			}
		}
		else if (strcmp(LR, XIP_RIGHT) == 0) {
			if (strcmp(UDLR, XIP_S_UP) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbRY > RIGHT_STICK_DEADZONE);
			}
			else if (strcmp(UDLR, XIP_S_DOWN) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbRY < -RIGHT_STICK_DEADZONE);
			}
			else if (strcmp(UDLR, XIP_S_LEFT) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbRX < -RIGHT_STICK_DEADZONE);
			}
			else if (strcmp(UDLR, XIP_S_RIGHT) == 0) {
				return (Controller[cNum].State.Gamepad.sThumbRX > RIGHT_STICK_DEADZONE);
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックのトリガー状態を取得
//=============================================================================
SHORT GetControllerStickTrigger(LPCSTR LR, LPCSTR UDLR, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if (strcmp(LR, XIP_LEFT) == 0) {
			if (strcmp(UDLR, XIP_S_UP) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLY > LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLY > LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbLY > LEFT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_DOWN) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_LEFT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_RIGHT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLX > LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLX > LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbLX > LEFT_STICK_DEADZONE);
				}
			}
		}
		else if (strcmp(LR, XIP_RIGHT) == 0) {
			if (strcmp(UDLR, XIP_S_UP) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRY > RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRY > RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbRY > RIGHT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_DOWN) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRY < -RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRY < -RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbRY < -RIGHT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_LEFT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRX < -RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRX < -RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbRX < -RIGHT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_RIGHT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRX > RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRX > RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].State.Gamepad.sThumbRX > RIGHT_STICK_DEADZONE);
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックのリリース状態を取得
//=============================================================================
SHORT GetControllerStickRelease(LPCSTR LR, LPCSTR UDLR, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if (strcmp(LR, XIP_LEFT) == 0) {
			if (strcmp(UDLR, XIP_S_UP) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLY > LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLY > LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbLY > LEFT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_DOWN) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_LEFT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_RIGHT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbLX > LEFT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbLX > LEFT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbLX > LEFT_STICK_DEADZONE);
				}
			}
		}
		else if (strcmp(LR, XIP_RIGHT) == 0) {
			if (strcmp(UDLR, XIP_S_UP) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRY > RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRY > RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbRY > RIGHT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_DOWN) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRY < -RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRY < -RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbRY < -RIGHT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_LEFT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRX < -RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRX < -RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbRX < -RIGHT_STICK_DEADZONE);
				}
			}
			else if (strcmp(UDLR, XIP_S_RIGHT) == 0) {
				if ((Controller[cNum].State.Gamepad.sThumbRX > RIGHT_STICK_DEADZONE) != (Controller[cNum].Old.Gamepad.sThumbRX > RIGHT_STICK_DEADZONE)) {
					return (Controller[cNum].Old.Gamepad.sThumbRX > RIGHT_STICK_DEADZONE);
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// スティックの軸の状態を取得
//=============================================================================
SHORT GetControllerStickAxis(LPCSTR LR, LPCSTR Axis, BYTE cNum) {
	if (Controller[cNum].Connected) {
		if (strcmp(LR, XIP_LEFT) == 0) {
			float magnitude = sqrtf((float)Controller[cNum].State.Gamepad.sThumbLX * (float)Controller[cNum].State.Gamepad.sThumbLX + 
				(float)Controller[cNum].State.Gamepad.sThumbLY * (float)Controller[cNum].State.Gamepad.sThumbLY);
			if (magnitude > 32767) {
				magnitude = 32767;
			}
			if (magnitude > LEFT_STICK_DEADZONE) {
				if (strcmp(Axis, XIP_S_X) == 0) {
					return (Controller[cNum].State.Gamepad.sThumbLX);
				}
				else if (strcmp(Axis, XIP_S_Y) == 0) {
					return (Controller[cNum].State.Gamepad.sThumbLY);
				}
			}
		}
		else if (strcmp(LR, XIP_RIGHT) == 0) {
			float magnitude = sqrtf((float)Controller[cNum].State.Gamepad.sThumbRX * (float)Controller[cNum].State.Gamepad.sThumbRX +
				(float)Controller[cNum].State.Gamepad.sThumbRY * (float)Controller[cNum].State.Gamepad.sThumbRY);
			if (magnitude > 32767) {
				magnitude = 32767;
			}
			if (magnitude > RIGHT_STICK_DEADZONE) {
				if (strcmp(Axis, XIP_S_X) == 0) {
					return (Controller[cNum].State.Gamepad.sThumbRX);
				}
				else if (strcmp(Axis, XIP_S_Y) == 0) {
					return (Controller[cNum].State.Gamepad.sThumbRY);
				}
			}
		}
	}
	return 0;
}

//=============================================================================
// バイブレーション設定
//=============================================================================
void SetControllerVibration(LPCSTR LRB, WORD Pow, BYTE cNum) {
	if (Controller[cNum].Connected) {
		SecureZeroMemory(&Controller[cNum].Vibration, sizeof(XINPUT_VIBRATION));	// バイブレーション情報のメモリ初期化
		if (strcmp(LRB, XIP_LEFT) == 0) {
			Controller[cNum].Vibration.wLeftMotorSpeed = Pow;
		}
		else if (strcmp(LRB, XIP_RIGHT) == 0) {
			Controller[cNum].Vibration.wRightMotorSpeed = Pow;
		}
		else if (strcmp(LRB, XIP_BOTH) == 0) {
			Controller[cNum].Vibration.wLeftMotorSpeed = Pow;
			Controller[cNum].Vibration.wRightMotorSpeed = Pow;
		}
		XInputSetState(cNum, &Controller[cNum].Vibration);
	}
}

//=============================================================================
// バッテリー情報取得
//=============================================================================
BYTE GetControllerBattery(BYTE cNum) {
	if (Controller[cNum].Connected) {
		return (Controller[cNum].Battery.BatteryLevel);
	}
	return 0;
}