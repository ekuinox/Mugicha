#pragma once

#include "always_displayed_polygon.h"
#include "conf.h"
#include <map>

// �v���C���[�N���X

class Selector : public AlwaysDisplayedPolygon
{
private:
	char selection;
public:
	Selector(LPDIRECT3DTEXTURE9 _tex);
	~Selector();
	void update();
	// �Ǝ�

	void init();
	void left();
	void right();
	char get_selection();
};