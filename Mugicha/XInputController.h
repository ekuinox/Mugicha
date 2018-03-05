#ifndef INCLUDED_XINPUTCONTROLLER
#define INCLUDED_XINPUTCONTROLLER

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <Windows.h>
#include <XInput.h>
#include <string.h>
#include <math.h>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment(lib, "XInput.lib")

//*****************************************************************************
// マクロの定義
//*****************************************************************************
/* コントローラー */
enum ControllerNumber{
	CNUM_1,	// コントローラー1
	CNUM_2,	// コントローラー2
	CNUM_3,	// コントローラー3
	CNUM_4	// コントローラー4
};

/* ガイドボタン(DLL使用時専用) */
#define XINPUT_GAMEPAD_GUIDE	0x0400

/* ボタン */
#define XIP_D_UP			XINPUT_GAMEPAD_DPAD_UP			// ↑ボタン
#define XIP_D_DOWN			XINPUT_GAMEPAD_DPAD_DOWN		// ↓ボタン
#define XIP_D_LEFT			XINPUT_GAMEPAD_DPAD_LEFT		// ←ボタン
#define XIP_D_RIGHT			XINPUT_GAMEPAD_DPAD_RIGHT		// →ボタン
#define XIP_A				XINPUT_GAMEPAD_A				// Aボタン
#define XIP_B				XINPUT_GAMEPAD_B				// Bボタン
#define XIP_X				XINPUT_GAMEPAD_X				// Xボタン
#define XIP_Y				XINPUT_GAMEPAD_Y				// Yボタン
#define XIP_START			XINPUT_GAMEPAD_START			// STARTボタン
#define XIP_BACK			XINPUT_GAMEPAD_BACK				// BACKボタン
#define XIP_LSB				XINPUT_GAMEPAD_LEFT_THUMB		// 左スティックボタン
#define XIP_RSB				XINPUT_GAMEPAD_RIGHT_THUMB		// 右スティックボタン
#define XIP_LB				XINPUT_GAMEPAD_LEFT_SHOULDER	// Lボタン
#define XIP_RB				XINPUT_GAMEPAD_RIGHT_SHOULDER	// Rボタン
#define XIP_GUIDE			XINPUT_GAMEPAD_GUIDE			// ガイドボタン(DLL使用時専用)

/* 左右識別 */
#define XIP_LEFT			"XIP_LEFT"			// 左
#define XIP_RIGHT			"XIP_RIGHT"			// 右
#define XIP_BOTH			"XIP_BOTH"			// 両方(バイブレーション専用)

/* スティック */
#define XIP_S_UP			"XIP_S_UP"			// スティック↑
#define XIP_S_DOWN			"XIP_S_DOWN"		// スティック↓
#define XIP_S_LEFT			"XIP_S_LEFT"		// スティック←
#define XIP_S_RIGHT			"XIP_S_RIGHT"		// スティック→
#define XIP_S_X				"XIP_S_X"			// スティックX軸
#define XIP_S_Y				"XIP_S_Y"			// スティックY軸

/* バイブレーション */
#define XIP_VIBRATION_MAX	65535				// モーターの最大振動
#define XIP_VIBRATION_HIGH	65535/4*3			// モーターの強振動
#define XIP_VIBRATION_MID	65535/2				// モーターの中振動
#define XIP_VIBRATION_LOW	65535/4				// モーターの弱振動
#define XIP_VIBRATION_OFF	0					// モーターの停止

/* バッテリー */
#define XIP_BATTERY_FULL	BATTERY_LEVEL_FULL		// バッテリー満
#define XIP_BATTERY_MEDIUM	BATTERY_LEVEL_MEDIUM	// バッテリー中
#define XIP_BATTERY_LOW		BATTERY_LEVEL_LOW		// バッテリー小
#define XIP_BATTERY_EMPTY	BATTERY_LEVEL_EMPTY		// バッテリー空

//*****************************************************************************
// グローバル
//*****************************************************************************
extern	BYTE	LEFT_TRIGGER_DEADZONE;		// 左トリガーのしきい値
extern	BYTE	RIGHT_TRIGGER_DEADZONE;		// 右トリガーのしきい値
extern	USHORT	LEFT_STICK_DEADZONE;		// 左スティックのしきい値
extern	USHORT	RIGHT_STICK_DEADZONE;		// 右スティックのしきい値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void	LoadXInputDLL(void);	// XInputDLLのロード
void	FreeXInputDLL(void);	// XInputDLLの解放

HRESULT	UpdateController(void);	// コントローラー更新処理

bool	GetControllerButtonPress(WORD Btn, BYTE cNum = CNUM_1);		// ボタンのプレス状態を取得
bool	GetControllerButtonTrigger(WORD Btn, BYTE cNum = CNUM_1);	// ボタンのトリガー状態を取得
bool	GetControllerButtonRelease(WORD Btn, BYTE cNum = CNUM_1);	// ボタンのリリース状態を取得

BYTE	GetControllerTriggerPress(LPCSTR LR, BYTE cNum = CNUM_1);	// トリガーのプレス状態を取得
BYTE	GetControllerTriggerTrigger(LPCSTR LR, BYTE cNum = CNUM_1);	// トリガーのトリガー状態を取得
BYTE	GetControllerTriggerRelease(LPCSTR LR, BYTE cNum = CNUM_1);	// トリガーのリリース状態を取得

SHORT	GetControllerStickPress(LPCSTR LR, LPCSTR UDLR, BYTE cNum = CNUM_1);	// スティックのプレス状態を取得
SHORT	GetControllerStickTrigger(LPCSTR LR, LPCSTR UDLR, BYTE cNum = CNUM_1);	// スティックのトリガー状態を取得
SHORT	GetControllerStickRelease(LPCSTR LR, LPCSTR UDLR, BYTE cNum = CNUM_1);	// スティックのリリース状態を取得
SHORT	GetControllerStickAxis(LPCSTR LR, LPCSTR Axis, BYTE cNum = CNUM_1);		// スティックの軸の状態を取得

void	SetControllerVibration(LPCSTR LRB, WORD Pow, BYTE cNum = CNUM_1);	// バイブレーション設定

BYTE	GetControllerBattery(BYTE cNum = CNUM_1);	// バッテリー情報取得

#endif