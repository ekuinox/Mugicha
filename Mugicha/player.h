#pragma once

#ifdef _DEBUG_
#define _PLAYER_DEBUG
#endif

#include "plain_square_polygon.h"
#include "conf.h"
#include <map>

// プレイヤークラス

class Player : public PlainSquarePolygon
{
private:
	bool alive; // 生死
	bool jumping; // ジャンプしている
	bool ground; // 着地状態
	bool moving; // 動いている(x軸)
	DWORD jumped_at;
	bool controll_lock; // 操作を受け付けなくする
	POLSIZE zoom_level; // ズームサイズ
	std::map<enum PolygonTypes, std::vector<SquarePolygonBase*>> &polygons; // 当たり見るように持っときます
	void generate_vertexes();
public:
	Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera, std::map<enum PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, int _layer = 0, float _x = PLAYER_X, float _y = PLAYER_Y, float _w = PLAYER_WIDTH, float _h = PLAYER_HEIGHT, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Player();
	
	void init();
	void update();
	void zoom(POLSIZE _zoom_level);

	// 独自関数
	bool jump();
	void lock();
	void unlock();
	void kill();
	bool dead();
};