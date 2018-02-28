#pragma once

#include "scalable_object.h"

// アッツいマグマのクラス
// 当たるとプレイヤは死ぬぷに～

class Magma : public ScalableObject
{
public:
	Magma(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0);
private:
};