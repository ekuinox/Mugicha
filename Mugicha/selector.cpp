#include "selector.h"

// �\���ɂ����g��Ȃ��̂�
Selector::Selector(LPDIRECT3DTEXTURE9 _tex)
	: AlwaysDisplayedPolygon(0, 0, 100, 100, _tex, 0)
{
	init();
}

Selector::~Selector()
{
}

void Selector::update()
{
	unless(status) return;
	// selection����x�𓱂�
	x = selection * SCREEN_WIDTH / 4;
}

void Selector::init()
{
	x = SCREEN_WIDTH / 4;
	y = SCREEN_HEIGHT * 0.8f;
	drawing = false;
	status = false;
	selection = 1;
}

void Selector::left()
{
	if (selection < 2) return;
	selection -= 1;
}

void Selector::right()
{
	if (selection > 2) return;
	selection += 1;
}

char Selector::get_selection()
{
	return selection;
}
