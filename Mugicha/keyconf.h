#pragma once

#include "input.h"
#include "XInputController.h"

// key conf

#define PLAYER_MOVE_LEFT (GetKeyboardPress(DIK_A) || GetControllerButtonPress(XIP_D_LEFT)) // 左移動
#define PLAYER_MOVE_RIGHT (GetKeyboardPress(DIK_D) || GetControllerButtonPress(XIP_D_RIGHT)) // 右移動
#define PLAYER_JUMP (GetKeyboardTrigger(DIK_SPACE) || GetControllerButtonTrigger(XIP_A)) // ジャンプ
#define PLAYER_JUMP_HOLD (GetKeyboardPress(DIK_SPACE) || GetControllerButtonPress(XIP_A)) // ジャンプ
#define PLAYER_ITEM_USE (GetKeyboardTrigger(DIK_I) || GetControllerButtonTrigger(XIP_B)) //  アイテムを掴んだり離したり

#define ZOOM_MIN (GetKeyboardTrigger(DIK_L) || GetControllerButtonTrigger(XIP_LB)) // 最小
#define ZOOM_DEF (GetKeyboardTrigger(DIK_K) || GetControllerButtonTrigger(XIP_X)) // 普通
#define ZOOM_MAX (GetKeyboardTrigger(DIK_J) || GetControllerButtonTrigger(XIP_RB)) // 最大

#define KEY_BACK_TO_TITLE (GetKeyboardTrigger(DIK_F5) || GetControllerButtonTrigger(XIP_START)) // タイトルに戻るためのキー