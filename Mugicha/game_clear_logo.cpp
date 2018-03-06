#include "game_clear_logo.h"
#include "helpers.h"

GameClearLogo::GameClearLogo(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 & _camera)
	: PlainSquarePolygon(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, GAMECLEAR_LOGO_DEFAULT.w, GAMECLEAR_LOGO_DEFAULT.h, _tex, 0, _camera, 0, 0, 1, 1)
{
}

void GameClearLogo::update()
{
	if (!status) return;

	auto current = SCNOW;
	if (time_diff(latest_update, current) > UPDATE_INTERVAL)
	{
		latest_update = current;

		if (w < GAMECLEAR_LOGO_MAX.w)
		{
			w += GAMECLEAR_LOGO_ZOOM_SPEED;
			h += GAMECLEAR_LOGO_ZOOM_SPEED * (GAMECLEAR_LOGO_MAX.h / GAMECLEAR_LOGO_MAX.w);
		}
	}

}
