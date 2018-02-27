#include "stage_background.h"

StageBackground::StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera)
	: PlainSquarePolygon(BACKGROUND_X, BACKGROUND_Y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, _tex, INT_MAX, camera, 0.0f, 0.5f, 0.5f, 0.5f)
{
}


StageBackground::~StageBackground()
{
}
