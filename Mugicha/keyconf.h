#pragma once

#include "input.h"
#include "XInputController.h"

// key conf

#define PLAYER_MOVE_LEFT (GetKeyboardPress(DIK_A) || GetControllerButtonPress(XIP_D_LEFT)) // ���ړ�
#define PLAYER_MOVE_RIGHT (GetKeyboardPress(DIK_D) || GetControllerButtonPress(XIP_D_RIGHT)) // �E�ړ�
#define PLAYER_JUMP (GetKeyboardTrigger(DIK_SPACE) || GetControllerButtonTrigger(XIP_A)) // �W�����v
#define PLAYER_JUMP_HOLD (GetKeyboardPress(DIK_SPACE) || GetControllerButtonPress(XIP_A)) // �W�����v
#define PLAYER_ITEM_USE (GetKeyboardPress(DIK_I) || GetControllerButtonPress(XIP_RB)) //  �A�C�e����͂񂾂藣������

#define ZOOM_MIN (GetKeyboardTrigger(DIK_L) || GetControllerButtonTrigger(XIP_B)) // �ŏ�
#define ZOOM_DEF (GetKeyboardTrigger(DIK_K) || GetControllerButtonTrigger(XIP_Y)) // ����
#define ZOOM_MAX (GetKeyboardTrigger(DIK_J) || GetControllerButtonTrigger(XIP_X)) // �ő�

#define KEY_BACK_TO_TITLE (GetKeyboardTrigger(DIK_F5) || GetControllerButtonTrigger(XIP_START)) // �^�C�g���ɖ߂邽�߂̃L�[