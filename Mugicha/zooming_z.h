#pragma once
#include "plain_square_polygon.h"

#define ZOOMING_Z_X (150) // 初期位置Z
#define ZOOMING_Z_Y (500) // 初期位置Y
#define ZOOMING_Z_SIZE (300) // 初期サイズ
#define ZOOMING_Z_MAX (500) // 最大サイズ
#define ZOOMING_Z_MIN (100) // 最小サイズ
#define ZOOMING_Z_SPEED (2.0f) // 拡縮速度

// ZoomingのうごくZだZ

class ZoomingZ : public PlainSquarePolygon
{
public:
	ZoomingZ(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera);
	void update();
private:
	bool plus;
};

