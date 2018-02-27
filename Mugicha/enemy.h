#pragma once

#include "scalable_object.h"
#include "player.h"

// 拡縮しちゃう敵さん用のクラスです
// いまはまだ何もない

class Enemy : public ScalableObject
{
public:
	enum class Vec{ LEFT, RIGHT}; // 進む向き
private:
	bool alive; // 生死
	bool on_ground; // 接地
	bool jumping; // ジャンプしている
	bool moving; // 行動
	DWORD jumped_at; // ジャンプした時点の時刻
	Vec vec;
	Player *player;
	std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &polygons;
public:
	Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, int _layer, float _x, float _y, float _w, float _h, Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
};