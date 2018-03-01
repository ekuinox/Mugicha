#include "gimmick_switch.h"

GimmickSwitch::GimmickSwitch(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, LPDIRECT3DTEXTURE9 _bullet_tex, int _layer, D3DXVECTOR2 & _camera, bool & _flag, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), flag(_flag)
{
}

void GimmickSwitch::press()
{
	flag = true;
}

void GimmickSwitch::release()
{
	flag = false;
}
