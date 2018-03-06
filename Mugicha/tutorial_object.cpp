#include "tutorial_object.h"
#include "helpers.h"

TutorialObject::TutorialObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 & _camera, CELL one, CELL two)
	: ScalableObject(_x, _y, _w, _h, _tex, TUTORIALS_DEFAULT_LAYER, _camera, one.x * TUTORIAL_TEXTURE_UV_W, one.y * TUTORIAL_TEXTURE_UV_H, TUTORIAL_TEXTURE_UV_W, TUTORIAL_TEXTURE_UV_H), uv_sel(0)
{
	uvs[0] = one;
	uvs[1] = two;
}

void TutorialObject::update()
{
	if (!status) return;

	auto current = SCNOW;
	if (time_diff(latest_update, current) > TEXTURE_ANIMATION_CHANGE_INTERVAL)
	{
		latest_update = current;

		uv_sel = uv_sel == 0 ? 1 : 0;

		u = uvs[uv_sel].x * TUTORIAL_TEXTURE_UV_W;
		v = uvs[uv_sel].y * TUTORIAL_TEXTURE_UV_H;
	}

}
