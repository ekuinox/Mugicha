#include "magma.h"
#include "collision_checker.h"

Magma::Magma(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, float _u, float _v, float _uw, float _vh)
	: ScalableObject(_x, _y, _w, _h, _tex, _layer, _camera, _u, _v, _uw, _vh)
{
}

SQUARE Magma::get_square()
{
	return SQUARE(x, y, w, h) * zoom_level;
}
