#pragma once

#include "plain_square_polygon.h"
#include "conf.h"

// �v���C���[�N���X

class Player : public PlainSquarePolygon
{
private:
	bool jumping; // �W�����v���Ă���
	bool falling; // �����Ă���
	bool moving; // �����Ă���(x��)
public:
	Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera, int _layer = 0, float _x = PLAYER_X, float _y = PLAYER_Y, float _w = PLAYER_WIDTH, float _h = PLAYER_HEIGHT, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Player();
	void update();
};