#pragma once

#include "polygonsquare.h"
#include "conf.h"

/*
* PLAYER
* SquarePolygonBaseを継承した簡単なポリゴンクラス
*/

class Player : public SquarePolygonBase
{
private:
	void generate_vertexes();
	bool jumping; // ジャンプしている
	bool falling; // 落ちている
	bool moving; // 動いている(x軸)
public:
	Player(LPDIRECT3DTEXTURE9 _tex, int _layer = 0, float _x = PLAYER_X, float _y = PLAYER_Y, float _w = PLAYER_WIDTH, float _h = PLAYER_HEIGHT, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Player();
	void update();
	void draw();
	bool is_drawing();
	void switch_drawing(bool _drawing);
	void switch_drawing();
	void change_texture(LPDIRECT3DTEXTURE9 _tex);
	bool is_collision(SquarePolygonBase *pol);
	void switch_status(bool _status);
	bool is_active();
};