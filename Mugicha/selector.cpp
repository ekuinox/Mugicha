#include "selector.h"

// ソレにしか使わないので
Selector::Selector(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera)
	: PlainSquarePolygon(0, 0, 100, 100, _tex, 0, _camera)
{
	init();
}

Selector::~Selector()
{
}

void Selector::update()
{
	unless(status) return;
	// selectionからxを導く
	x = selection * 200;
}

void Selector::init()
{
	x = 200;
	y = 600;
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
