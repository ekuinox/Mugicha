#pragma once

#define unless(expression) if(!(expression))

/* Constant */
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "Zooming"


// 固定の設定をここにベタ書きする

// スクリーンのデカさ
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (800)

// プレイヤのデカさ
#define PLAYER_WIDTH (50)
#define PLAYER_HEIGHT (50)
#define PLAYER_X (300)
#define PLAYER_Y (50)

// 背景のデカさ
#define BACKGROUND_WIDTH (SCREEN_WIDTH)
#define BACKGROUND_HEIGHT (SCREEN_HEIGHT)
#define BACKGROUND_X (BACKGROUND_WIDTH / 2)
#define BACKGROUND_Y (BACKGROUND_HEIGHT / 2)

// カメラ位置 => よくわからんけどプレイヤに追従するよう考える，これは初期位置
// 各ポリゴンがvertexesを生成するときに参照され，これを引く
#define CAMERA_X (0)
#define CAMERA_Y (0)

#define B_TO_S(exp) exp ? "TRUE" : "FALSE"