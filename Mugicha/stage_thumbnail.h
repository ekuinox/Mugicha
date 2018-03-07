#pragma once
#include "plain_square_polygon.h"

#define STAGE_THUMBNAIL_SIZE (300)
#define STAGE_THUMBNAIL_TRIGGERD_SIZE_MAX (400)
#define STAGE_THUMBNAIL_TRIGGERD_SIZE_MIN (350)
#define STAGE_THUMBNAIL_SPEED (1.0f)
#define STAGE_THUMBNAIL_ZOOM_SPEED (1.0f)
#define STAGE_THUMBNAIL_SIZE_PLUS (true)
#define STAGE_THUMBNAIL_MOVES_DISTANCE (SCREEN_WIDTH)
#define STAGE_THUMBNAIL_LAYER (3)

// カーソルに選択されているときに拡大縮小をする

class StageThumbnail : public PlainSquarePolygon
{
public:
	StageThumbnail(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, float _x, float _y, float _w = STAGE_THUMBNAIL_SIZE, float _h = STAGE_THUMBNAIL_SIZE);
	void trigger();
	void release();
	bool is_triggered();
	void update();
private:
	D3DXVECTOR2 init_coords;
	bool plus;
	bool triggered;
	bool ready;
};