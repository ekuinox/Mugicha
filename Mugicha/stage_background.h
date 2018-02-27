#pragma once
#include "plain_square_polygon.h"

/*
* StageBackground
* ステージの背景にのみ使う
* x,y,w,hの変更は行わない
* UV値で背景をスクロールさせていく．
* 他と違い，これのUV値はXYと同じように中心点から求める
*/

class StageBackground : public PlainSquarePolygon
{
public:
	using UVSet = struct _UVSet {
		float u, v, uw, vh;
		_UVSet() {}
		_UVSet(float _u, float _v, float _uw, float _vh)
			: u(_u), v(_v), uw(_uw), vh(_vh) {}
	};
	StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera);
	StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, UVSet _uvset);
	~StageBackground();
	void update(); 
private:
	void generate_vertexes();
	POLSIZE zoom_level;
	UVSet uvset;
};

