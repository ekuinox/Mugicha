#pragma once
#include "always_displayed_polygon.h"

#define LIFEGAGE_TEXTURE_UV_W (0.5f)
#define LIFEGAGE_TEXTURE_UV_X_SPEED (0.25f)
#define LIFE_GAGE_WIDTH (100)
#define LIFE_GAGE_HEIGHT (50)
#define LIFE_GAGE_X (100)
#define LIFE_GAGE_Y (60)

class LifeGage : public AlwaysDisplayedPolygon
{
public:
	LifeGage(LPDIRECT3DTEXTURE9 _tex, char &_life);
	void update();
private:
	char &life;
};

