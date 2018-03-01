#include "stage_background.h"

void StageBackground::generate_vertexes()
{
	for (auto i = 0; i < 4; ++i)
	{
		vertexes[i] = VERTEX_2D(
			x + w / (i % 3 == 0 ? -2 : 2),
			y + h / (i < 2 ? -2 : 2),
			u + uw / (i % 3 == 0 ? -2 : 2),
			v + vh / (i < 2 ? -2 : 2)
		);
	}
}

StageBackground::StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera)
	: PlainSquarePolygon(BACKGROUND_X, BACKGROUND_Y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, _tex, INT_MAX, _camera), uvset(0.1f, 0.1f, 0.01f, 0.01f)
{
	zoom_level = { 1, 1 };
}

StageBackground::StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 & _camera, UVSet _uvset)
	: PlainSquarePolygon(BACKGROUND_X, BACKGROUND_Y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, _tex, INT_MAX, _camera), uvset(_uvset)
{
}


StageBackground::~StageBackground()
{
}

void StageBackground::update()
{
	if (!status) return;

	auto current = std::chrono::system_clock::now();

	auto uv_vec = D3DXVECTOR2();

	u = camera.x / SCREEN_WIDTH / zoom_level.w * uvset.u;
	v = 1.0f - (camera.y / SCREEN_HEIGHT / zoom_level.h * uvset.v);

	uw = uvset.uw * zoom_level.w;
	vh = uvset.vh * zoom_level.h;
}
