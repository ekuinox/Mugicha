#pragma once
#include "scalable_object.h"

/*
*
* GimmickSwitch
* 適当なフラグを持たせて，このクラスでオンにしたときにそのフラグもオンにしてやる
* そういう感じ
*/

class GimmickSwitch : public ScalableObject
{
public:
	GimmickSwitch(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, bool &_flag, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void press(); // onにする
	void release(); // offにする
private:
	bool &flag; // スイッチが有効になったときにオンにするフラグをここにいれます
};