#include "life_gage.h"
#include "helpers.h"

LifeGage::LifeGage(LPDIRECT3DTEXTURE9 _tex, char & _life)
	: AlwaysDisplayedPolygon(LIFE_GAGE_X, LIFE_GAGE_Y, LIFE_GAGE_WIDTH, LIFE_GAGE_HEIGHT, _tex, 0, 0, 0, LIFEGAGE_TEXTURE_UV_W, 1), life(_life)
{
}

void LifeGage::update()
{
	if (!status) return;
	
	auto current = SCNOW;
	if (time_diff(latest_update, current) > UPDATE_INTERVAL)
	{
		latest_update = current;

		u = LIFEGAGE_TEXTURE_UV_X_SPEED * (2 - life);
	}
}
