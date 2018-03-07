#pragma once
#include "always_displayed_polygon.h"

#define GAGE_WIDTH (40)
#define GAGE_HEIGHT (120)
#define GAGE_RECOVERY_SPEED (0.0005f) // ゲージ回復速度
#define GAGE_X (25)
#define GAGE_Y (70)

/*
* Gage
* 拡縮に使うコストのゲージ
*/

class Gage : public AlwaysDisplayedPolygon
{
public:
	Gage(LPDIRECT3DTEXTURE9 _tex);
	void update();
	float consume(); // 一回分消費して残量を返す，コストが足らない場合は負の値を返す
	bool can_consume();
#if defined(_DEBUG) || defined(_STAGE_DEBUG)
	void cure(); // 全回復させる
#endif
private:
	const float cost = 1.0f; // 一度に消費するコスト
	const float cost_gage_max = 2.0f; // コストの満タン状態
	const float cost_recovery_speed = GAGE_RECOVERY_SPEED; // コスト回復速度（毎ms）
	float current_cost; // 現在のコスト
};

