#pragma once
#include "always_displayed_polygon.h"

#define GAGE_WIDTH (100)
#define GAGE_HEIGHT (200)

/*
* Gage
* 拡縮に使うコストのゲージ
*/

class Gage : public AlwaysDisplayedPolygon
{
public:
	Gage(float _x, float _y, LPDIRECT3DTEXTURE9 _tex);
	void update();
	float consume(); // 一回分消費して残量を返す，コストが足らない場合は負の値を返す
	bool can_consume();
private:
	const float cost = 1.0f; // 一度に消費するコスト
	const float cost_gage_max = 2.0f; // コストの満タン状態
	const float cost_recovery_speed = 0.01f; // コスト回復速度（毎ms）
	float current_cost; // 現在のコスト
};

