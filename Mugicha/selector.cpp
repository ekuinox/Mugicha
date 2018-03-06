#include "selector.h"

// ƒ\ƒŒ‚É‚µ‚©Žg‚í‚È‚¢‚Ì‚Å
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
	// selection‚©‚çx‚ð“±‚­
	switch (selection)
	{
	case 1:
		x = 210;
		break;
	case 2:
		x = SCREEN_WIDTH / 2;
		break;
	case 3:
		x = SCREEN_WIDTH - 210;
		break;
	}
}

void Selector::init()
{
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - 100;
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
