#include "zooming_z.h"
#include "helpers.h"

ZoomingZ::ZoomingZ(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 & _camera)
	: PlainSquarePolygon(ZOOMING_Z_X, ZOOMING_Z_Y, ZOOMING_Z_SIZE, ZOOMING_Z_SIZE, _tex, 0, _camera), plus(true)
{
	speed = ZOOMING_Z_SPEED;
}

void ZoomingZ::update()
{
	if (!status) return;

	auto current = SCNOW;
	if (time_diff(latest_update, current) > UPDATE_INTERVAL)
	{
		latest_update = current;

		h = w += (plus ? speed : -speed);

		// Œü‚«‚ðƒLƒ‚Ü‚·
		if (plus && w > ZOOMING_Z_MAX) plus = false;
		else if (!plus && w < ZOOMING_Z_MIN) plus = true;
	}
}
