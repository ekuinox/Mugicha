#include "gimmick_switch.h"

GimmickSwitch::GimmickSwitch(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, bool & _flag, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), flag(_flag), pressed(false)
{
	def_y = y;
	def_h = h;
}

void GimmickSwitch::press()
{
	pressed = true;
	flag = true;
}

void GimmickSwitch::release()
{
	pressed = false;
	flag = false;
}

SQUARE GimmickSwitch::get_square()
{
	// ������Ă���Ƃ��͓����肪����
	return pressed ? SQUARE(x, y - CELL_HEIGHT / 4, w, h / 2) * zoom_level : ScalableObject::get_square();
}
