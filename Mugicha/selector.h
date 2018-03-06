#pragma once

#include "always_displayed_polygon.h"
#include "conf.h"
#include <map>

// プレイヤークラス

class Selector : public AlwaysDisplayedPolygon
{
private:
	char selection;
public:
	Selector(LPDIRECT3DTEXTURE9 _tex);
	~Selector();
	void update();
	// 独自

	void init();
	void left();
	void right();
	char get_selection();
};