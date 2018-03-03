#pragma once

// だまれバカ
#define _CRT_SECURE_NO_WARNINGS

// なんちゃってunlessなど
#define unless(expression) if(!(expression))
#define until(expression) while(!(expression))

// windowのアレ
#define CLASS_NAME "AppClass"
#define WINDOW_NAME "Zooming"

// 固定の設定をここにベタ書きする

// リソースがどこにあるか指定する
#define RESOURCES_DIR "./resources/"
#define TEXTURES_DIR RESOURCES_DIR "textures/"
#define STAGEFILES_DIR RESOURCES_DIR "stages/"

#define FRAME_RATES (60)
#define UPDATE_INTERVAL (1)

// スクリーンのデカさ
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define CELL_WIDTH (50)
#define CELL_HEIGHT (50)

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