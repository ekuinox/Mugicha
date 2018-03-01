#pragma once

#ifdef _DEBUG_
#define _PLAYER_DEBUG
#endif

#include "plain_square_polygon.h"
#include "conf.h"
#include <map>
#include "input.h"
#include "item.h"

// プレイヤークラス

class Player : public PlainSquarePolygon
{
public:
	enum class DeadReason
	{
		ALIVE, // 生きとるわい
		Sandwiched, // 挟まれて死ぬ
		HitEnemy, // 敵に接触
		HitThorn, // トゲに接触
		Falling, // 無に落下
		HitMagma, // マグマでアチチ
		Shot, // Bulletにドガーン
		FallenHellGate, // HellGate(仮称)に飲み込まれた
	};
	enum class Vec
	{
		CENTER,
		RIGHT,
		LEFT
	};
private:
	DeadReason dead_reason; // 生きているか，また死んでいるならその理由
	bool jumping; // ジャンプしている
	bool ground; // 着地状態
	bool moving; // 動いている(x軸)
	DWORD jumped_at;
	bool controll_lock; // 操作を受け付けなくする
	D3DXVECTOR2 when_locked_coords; // ロックされたときの座標値
	POLSIZE zoom_level; // ズームサイズ
	POLSIZE before_zoom_level; // 前のヤツ
	std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &polygons; // 当たり見るように持っときます
	Player::Vec vec;
	bool holding_item; // アイテムを持っている状態
	Item *item;

	bool collision_for_enemies();
	bool collision_for_thorns();
	bool collision_for_magmas();
	bool collision_for_bullets();
	bool collision_for_knockback_bullets(D3DXVECTOR2 &knockback);
	bool is_fallen_hellgate();
public:
	Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, int _layer, float _x, float _y, float _w, float _h, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Player();
	
	void init();
	void update();
	void zoom(POLSIZE _zoom_level);

	// 独自関数
	bool jump();
	bool catch_item();
	void release_item();
	bool is_holding_item();
	void lock();
	void unlock();
	void kill(const DeadReason &_dead_reason);
	Player::DeadReason dead();
	Player::Vec get_vec();
};