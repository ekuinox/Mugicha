#include "gimmick_floor.h"

GimmickFloor::GimmickFloor(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 & _camera, bool & _appearing, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh), appearing(_appearing)
{
}

void GimmickFloor::update()
{
	unless(status) return;

	if (appearing)
	{
		show();
	}
	else
	{
		hide();
	}
}

bool GimmickFloor::is_appearing()
{
	return appearing;
}
