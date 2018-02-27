#pragma once
#include "plain_square_polygon.h"

/*
* StageBackground
* ステージの背景にのみ使う
* x,y,w,hの変更は行わない
* UV値で背景をスクロールさせていく．
*/

class StageBackground : public PlainSquarePolygon
{
public:
	StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera);
	~StageBackground();
	void update();
};

