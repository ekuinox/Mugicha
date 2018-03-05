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
	GimmickSwitch(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, bool &_flag, float _u = 0.0f, float _v = 0.4f, float _uw = 0.5f, float _vh = 0.6f);
	void press(); // onにする
	void release(); // offにする
	SQUARE get_square();
private:
	bool pressed; // 押し込まれているフラグ
	bool &flag; // スイッチが有効になったときにオンにするフラグをここにいれます
	float def_y;
	float def_h;
};