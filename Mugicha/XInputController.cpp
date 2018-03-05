#include "XInputController.h"

//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define MAX_CONTROLLERS	4	// �ő�R���g���[���[��(XInput���F���ł���̂�4�܂�)
#define FNC_XINPUTGETSTATEEX	((const char*)100)	// XInputGetStateEX�p

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct CONTROLLER_STATE{
	XINPUT_STATE State;						// ���݂̓��͏��
	XINPUT_STATE Old;						// �O��̓��͏��
	XINPUT_VIBRATION Vibration;				// �o�C�u���[�V�������
	XINPUT_BATTERY_INFORMATION Battery;		// �o�b�e���[���
	bool Connected;							// �ڑ��m�F���
};

//*****************************************************************************
// �O���[�o��
//*****************************************************************************
CONTROLLER_STATE Controller[MAX_CONTROLLERS];	// �R���g���[���[�\����

BYTE	LEFT_TRIGGER_DEADZONE	= 255 / 10;		// ���g���K�[�̂������l
BYTE	RIGHT_TRIGGER_DEADZONE	= 255 / 10;		// �E�g���K�[�̂������l
USHORT	LEFT_STICK_DEADZONE		= 32767 / 3;	// ���X�e�B�b�N�̂������l
USHORT	RIGHT_STICK_DEADZONE	= 32767 / 3;	// �E�X�e�B�b�N�̂������l

/* XInputDLL */
using XINPUTGETSTATEEX = DWORD(WINAPI *)(DWORD dwUserIndex, XINPUT_STATE *pState);
XINPUTGETSTATEEX XInputGetStateEx = nullptr;
HMODULE XInput_dll = nullptr;

//=============================================================================
// XInputDLL�̃��[�h
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
// XInputDLL�̉��
//=============================================================================
void FreeXInputDLL(void) {
	if (XInput_dll != NULL) {
		::FreeLibrary(XInput_dll);
	}
}

//=============================================================================
// �R���g���[���[�X�V����
//=============================================================================
HRESULT UpdateController(void) {
	DWORD Result;

	for (BYTE i = 0; i < MAX_CONTROLLERS; i++) {
		SecureZeroMemory(&Controller[i].Old, sizeof(XINPUT_STATE));						// �O��̓��͏��̃�����������
		Controller[i].Old = Controller[i].State;										// ���݂̓��͏���O��̓��͏��֕ۑ�
		SecureZeroMemory(&Controller[i].State, sizeof(XINPUT_STATE));					// ���݂̓��͏��̃�����������
		SecureZeroMemory(&Controller[i].Battery, sizeof(XINPUT_BATTERY_INFORMATION));	// �o�b�e���[���̃�����������

		/* �R���g���[���[���擾 */
		if (XInput_dll == NULL) {									// XInputDLL�������ꍇ
			Result = XInputGetState(i, &Controller[i].State);		// ���͏��擾(�K�C�h�{�^���Ȃ�)
		}
		else {														// XInputDLL���L��ꍇ
			Result = XInputGetStateEx(i, &Controller[i].State);		// ���͏��擾(�K�C�h�{�^������)
		}
		XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &Controller[i].Battery);	// �o�b�e���[���擾

		/* �ڑ��m�F */
		if (Result == ERROR_SUCCESS) {
			Controller[i].Connected = true;		// �ڑ�����Ă���
		}
		else {
			Controller[i].Connected = false;	// �ڑ�����Ă��Ȃ�
		}
	}

	return S_OK;
}

//=============================================================================
// �{�^���̃v���X��Ԃ��擾
//=============================================================================
bool GetControllerButtonPress(WORD Btn, BYTE cNum) {
	if (Controller[cNum].Connected) {
		return (Controller[cNum].State.Gamepad.wButtons & Btn) != 0;
	}
	return 0;
}

//=============================================================================
// �{�^���̃g���K�[��Ԃ��擾
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
// �{�^���̃����[�X��Ԃ��擾
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
// �g���K�[�̃v���X��Ԃ��擾
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
// �g���K�[�̃g���K�[��Ԃ��擾
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
// �g���K�[�̃����[�X��Ԃ��擾
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
// �X�e�B�b�N�̃v���X��Ԃ��擾
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
// �X�e�B�b�N�̃g���K�[��Ԃ��擾
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
// �X�e�B�b�N�̃����[�X��Ԃ��擾
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
// �X�e�B�b�N�̎��̏�Ԃ��擾
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
// �o�C�u���[�V�����ݒ�
//=============================================================================
void SetControllerVibration(LPCSTR LRB, WORD Pow, BYTE cNum) {
	if (Controller[cNum].Connected) {
		SecureZeroMemory(&Controller[cNum].Vibration, sizeof(XINPUT_VIBRATION));	// �o�C�u���[�V�������̃�����������
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
// �o�b�e���[���擾
//=============================================================================
BYTE GetControllerBattery(BYTE cNum) {
	if (Controller[cNum].Connected) {
		return (Controller[cNum].Battery.BatteryLevel);
	}
	return 0;
}