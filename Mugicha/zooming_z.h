#pragma once
#include "plain_square_polygon.h"

#define ZOOMING_Z_X (150) // �����ʒuZ
#define ZOOMING_Z_Y (500) // �����ʒuY
#define ZOOMING_Z_SIZE (300) // �����T�C�Y
#define ZOOMING_Z_MAX (500) // �ő�T�C�Y
#define ZOOMING_Z_MIN (100) // �ŏ��T�C�Y
#define ZOOMING_Z_SPEED (2.0f) // �g�k���x

// Zooming�̂�����Z��Z

class ZoomingZ : public PlainSquarePolygon
{
public:
	ZoomingZ(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera);
	void update();
private:
	bool plus;
};

