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

/* ���͏����̏����� */
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

/* ���͏����̍X�V���� */
void UpdateInput()
{
	UpdateKeyboard();
}

/* ���͏����̏I������ */
void UninitInput()
{
	if (g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

/* �L�[�{�[�h�̏����� */
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	hr = InitInput(hInstance, hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����Ȃ��D", "X(", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h��������Ȃ��D", "X(", MB_ICONWARNING);
		return hr;
	}
	
	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�̃t�H�[�}�b�g�Ɏ��s�D", "X(", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��Ȃ������D", "X(", MB_ICONWARNING);
		return hr;
	}
	
	g_pDIDevKeyboard->Acquire();
	
	return S_OK;
}

/* �L�[�{�[�h�̍X�V���� */
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	if (SUCCEEDED(g_pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// �L�[�g���K�[�E�����[�X���𐶐��D
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & ~aKeyState[nCnKey];

			// �L�[�s�[�g���𐶐�
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

			// �L�[�v���X����ۑ�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDIDevKeyboard->Acquire();
	}
}

/* �L�[�{�[�h�̏I������ */
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

/* �L�[�{�[�h�̃v���X��Ԃ��擾 */
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

/* �L�[�{�[�h�̃g���K��Ԃ��擾 */
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

/* �L�[�{�[�h�̃��s�[�g��Ԃ��擾 */
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

/* �L�[�{�[�h�̃����[�X��Ԃ��擾 */
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}