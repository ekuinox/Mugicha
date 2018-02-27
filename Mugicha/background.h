#pragma once

#include "scalable_object.h"
#include "conf.h"

class Background : public ScalableObject {
public:
	Background(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	Background(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Background();
	void update();
};