#pragma once

#include "scalable_object.h"
#include "player.h"

// ボロい床のクラス
// 上にしばらく乗っていると落ちるというアレ．
// ある程度落としたら上に戻します

class RaggedFloor : public ScalableObject
{
private:
	Player *player; // 当たりをみるためのプレイヤ
	D3DXVECTOR2 init_pos; // 初期位置
	bool burdened; // 上にプレイヤが乗っているときにTRUE
	float burdened_time; // burdenedがTRUEの間加算する
	const float burdened_time_max = 5000; // いつまで耐えるか
	bool falling; // 落下しているときのフラグ
	float disapper_time; // 消しておく時間
	const float disapper_time_max = 5000; // いつまで消しておくか

	void init(); // 初期化に使う
public:
	RaggedFloor(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Player *_player, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
};