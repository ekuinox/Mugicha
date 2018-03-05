#ifndef INCLUDED_XINPUTCONTROLLER
#define INCLUDED_XINPUTCONTROLLER

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <Windows.h>
#include <XInput.h>
#include <string.h>
#include <math.h>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment(lib, "XInput.lib")

//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
/* �R���g���[���[ */
enum ControllerNumber{
	CNUM_1,	// �R���g���[���[1
	CNUM_2,	// �R���g���[���[2
	CNUM_3,	// �R���g���[���[3
	CNUM_4	// �R���g���[���[4
};

/* �K�C�h�{�^��(DLL�g�p����p) */
#define XINPUT_GAMEPAD_GUIDE	0x0400

/* �{�^�� */
#define XIP_D_UP			XINPUT_GAMEPAD_DPAD_UP			// ���{�^��
#define XIP_D_DOWN			XINPUT_GAMEPAD_DPAD_DOWN		// ���{�^��
#define XIP_D_LEFT			XINPUT_GAMEPAD_DPAD_LEFT		// ���{�^��
#define XIP_D_RIGHT			XINPUT_GAMEPAD_DPAD_RIGHT		// ���{�^��
#define XIP_A				XINPUT_GAMEPAD_A				// A�{�^��
#define XIP_B				XINPUT_GAMEPAD_B				// B�{�^��
#define XIP_X				XINPUT_GAMEPAD_X				// X�{�^��
#define XIP_Y				XINPUT_GAMEPAD_Y				// Y�{�^��
#define XIP_START			XINPUT_GAMEPAD_START			// START�{�^��
#define XIP_BACK			XINPUT_GAMEPAD_BACK				// BACK�{�^��
#define XIP_LSB				XINPUT_GAMEPAD_LEFT_THUMB		// ���X�e�B�b�N�{�^��
#define XIP_RSB				XINPUT_GAMEPAD_RIGHT_THUMB		// �E�X�e�B�b�N�{�^��
#define XIP_LB				XINPUT_GAMEPAD_LEFT_SHOULDER	// L�{�^��
#define XIP_RB				XINPUT_GAMEPAD_RIGHT_SHOULDER	// R�{�^��
#define XIP_GUIDE			XINPUT_GAMEPAD_GUIDE			// �K�C�h�{�^��(DLL�g�p����p)

/* ���E���� */
#define XIP_LEFT			"XIP_LEFT"			// ��
#define XIP_RIGHT			"XIP_RIGHT"			// �E
#define XIP_BOTH			"XIP_BOTH"			// ����(�o�C�u���[�V������p)

/* �X�e�B�b�N */
#define XIP_S_UP			"XIP_S_UP"			// �X�e�B�b�N��
#define XIP_S_DOWN			"XIP_S_DOWN"		// �X�e�B�b�N��
#define XIP_S_LEFT			"XIP_S_LEFT"		// �X�e�B�b�N��
#define XIP_S_RIGHT			"XIP_S_RIGHT"		// �X�e�B�b�N��
#define XIP_S_X				"XIP_S_X"			// �X�e�B�b�NX��
#define XIP_S_Y				"XIP_S_Y"			// �X�e�B�b�NY��

/* �o�C�u���[�V���� */
#define XIP_VIBRATION_MAX	65535				// ���[�^�[�̍ő�U��
#define XIP_VIBRATION_HIGH	65535/4*3			// ���[�^�[�̋��U��
#define XIP_VIBRATION_MID	65535/2				// ���[�^�[�̒��U��
#define XIP_VIBRATION_LOW	65535/4				// ���[�^�[�̎�U��
#define XIP_VIBRATION_OFF	0					// ���[�^�[�̒�~

/* �o�b�e���[ */
#define XIP_BATTERY_FULL	BATTERY_LEVEL_FULL		// �o�b�e���[��
#define XIP_BATTERY_MEDIUM	BATTERY_LEVEL_MEDIUM	// �o�b�e���[��
#define XIP_BATTERY_LOW		BATTERY_LEVEL_LOW		// �o�b�e���[��
#define XIP_BATTERY_EMPTY	BATTERY_LEVEL_EMPTY		// �o�b�e���[��

//*****************************************************************************
// �O���[�o��
//*****************************************************************************
extern	BYTE	LEFT_TRIGGER_DEADZONE;		// ���g���K�[�̂������l
extern	BYTE	RIGHT_TRIGGER_DEADZONE;		// �E�g���K�[�̂������l
extern	USHORT	LEFT_STICK_DEADZONE;		// ���X�e�B�b�N�̂������l
extern	USHORT	RIGHT_STICK_DEADZONE;		// �E�X�e�B�b�N�̂������l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void	LoadXInputDLL(void);	// XInputDLL�̃��[�h
void	FreeXInputDLL(void);	// XInputDLL�̉��

HRESULT	UpdateController(void);	// �R���g���[���[�X�V����

bool	GetControllerButtonPress(WORD Btn, BYTE cNum = CNUM_1);		// �{�^���̃v���X��Ԃ��擾
bool	GetControllerButtonTrigger(WORD Btn, BYTE cNum = CNUM_1);	// �{�^���̃g���K�[��Ԃ��擾
bool	GetControllerButtonRelease(WORD Btn, BYTE cNum = CNUM_1);	// �{�^���̃����[�X��Ԃ��擾

BYTE	GetControllerTriggerPress(LPCSTR LR, BYTE cNum = CNUM_1);	// �g���K�[�̃v���X��Ԃ��擾
BYTE	GetControllerTriggerTrigger(LPCSTR LR, BYTE cNum = CNUM_1);	// �g���K�[�̃g���K�[��Ԃ��擾
BYTE	GetControllerTriggerRelease(LPCSTR LR, BYTE cNum = CNUM_1);	// �g���K�[�̃����[�X��Ԃ��擾

SHORT	GetControllerStickPress(LPCSTR LR, LPCSTR UDLR, BYTE cNum = CNUM_1);	// �X�e�B�b�N�̃v���X��Ԃ��擾
SHORT	GetControllerStickTrigger(LPCSTR LR, LPCSTR UDLR, BYTE cNum = CNUM_1);	// �X�e�B�b�N�̃g���K�[��Ԃ��擾
SHORT	GetControllerStickRelease(LPCSTR LR, LPCSTR UDLR, BYTE cNum = CNUM_1);	// �X�e�B�b�N�̃����[�X��Ԃ��擾
SHORT	GetControllerStickAxis(LPCSTR LR, LPCSTR Axis, BYTE cNum = CNUM_1);		// �X�e�B�b�N�̎��̏�Ԃ��擾

void	SetControllerVibration(LPCSTR LRB, WORD Pow, BYTE cNum = CNUM_1);	// �o�C�u���[�V�����ݒ�

BYTE	GetControllerBattery(BYTE cNum = CNUM_1);	// �o�b�e���[���擾

#endif