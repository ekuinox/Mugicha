#pragma once

#include "plain_square_polygon.h"
#include "conf.h"
#include <map>

// �v���C���[�N���X

class Selector : public PlainSquarePolygon
{
private:
	char selection;
public:
	Selector(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera);
	~Selector();
	void update();
	// �Ǝ�

	void init();
	void left();
	void right();
	char get_selection();
};