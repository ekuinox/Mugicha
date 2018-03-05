#pragma once

#include "plain_square_polygon.h"
#include "conf.h"
#include <map>
#include "input.h"
#include "XInputController.h"
#include "item.h"
#include "helpers.h"
#include "audio_controller.h"
#include <memory>

// 初期値など
#define PLAYER_SPEED (0.675f) // プレイや移動速度
#define PLAYER_JUMP_POWER (2.0f) // ジャンプ力，1msにどんだけ飛ぶか
#define PLAYER_JUMP_HEIGHT (CELL_HEIGHT * 2.5) // ジャンプする高さ
#define PLAYER_HOLD_JUMP_HEIGHT (CELL_HEIGHT * 4.0) // 長押しジャンプする高さ
#define PLAYER_FALLING (1.0f) // 落下速度
#define PLAYER_WIDTH (CELL_WIDTH * 0.8) // プレイヤの幅
#define PLAYER_HEIGHT (CELL_HEIGHT * 0.8) // プレイヤの高さ
#define COLLISION_CHECK_POLYGONTYPES { \
	SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, \
	SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, \
	SquarePolygonBase::PolygonTypes::THORN, \
	SquarePolygonBase::PolygonTypes::AIRCANNON, \
	SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH \
	} // 当たり判定を取るポリゴンのタイプまとめ
#define FALLING_OUT_Y (-10) // 画面外落下の高さ
#define KNOCKBACK_VOLUME ((CELL_WIDTH + CELL_HEIGHT) / 2) // ノックバック距離

// プレイヤークラス

class Player : public PlainSquarePolygon
{
public:
	// 死因列挙
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
	// 向き列挙
	enum class Vec
	{
		CENTER,
		RIGHT,
		LEFT
	};
private:
	PolygonsContainer &polygons; // 当たり見るように持っときます

	DeadReason dead_reason; // 生きているか，また死んでいるならその理由
	
	bool jumping; // ジャンプしている
	float jumped_at;
	bool ground; // 着地状態
	bool moving; // 動いている(x軸)
	
	bool controll_lock; // 操作を受け付けなくする
	D3DXVECTOR2 when_locked_coords; // ロックされたときの座標値
	
	float zoom_level; // ズームサイズ
	float before_zoom_level; // 前のヤツ

	Player::Vec vec;
	Player::Vec old_vec;
	
	Item *item; // 持っているアイテムを格納する．持っていない場合はnullptrにする．
	
	AudioController *audiocontroller;

	bool collision_for_enemies();
	bool collision_for_thorns();
	bool collision_for_magmas();
	bool collision_for_bullets();
	D3DXVECTOR2 collision_for_knockback_bullets();
	void collision_for_knockback_bullets(D3DXVECTOR2 &vector, char &result);
	bool is_fallen_hellgate();
	char collision_check();
	void sandwich_check(char &result); // 挟まれてるか
	void falling_out_check(char &result); // 画面外にまで落下していないか
	void ground_check(char &result); // 着地しているか
	void head_check(char &result); // 頭打ってないか
	void controlls(D3DXVECTOR2 &vector, char &result);
	void jump(D3DXVECTOR2 &vector, char &result);
	void drifting(D3DXVECTOR2 &vector); // 落下させる
	bool catch_item(); // アイテムをつかむ
	void release_item(); // アイテムを離す
	bool is_holding_item(); // アイテムを持っているか

	void generate_vertexes();
public:
	Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, PolygonsContainer &_polygons, int _layer, float _x, float _y, float _w = PLAYER_WIDTH, float _h = PLAYER_HEIGHT, float _u = 0.0f, float _v = 0.0f, float _uw = 0.1f, float _vh = 0.25f);
	~Player();
	void init();
	void update();
	void zoom(float _zoom_level);

	// 独自関数
	
	void lock();
	void unlock();
	void kill(const DeadReason &_dead_reason);
	bool is_jumping();
	void trigger_controlls();
	Player::DeadReason dead();
	Player::Vec get_vec();
	SQUARE get_square();
};