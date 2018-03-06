#pragma once
#include "plain_square_polygon.h"

#define GAMECLEAR_LOGO_MAX (POLSIZE(SCREEN_WIDTH, SCREEN_HEIGHT))
#define GAMECLEAR_LOGO_DEFAULT (POLSIZE(0, 0))
#define GAMECLEAR_LOGO_ZOOM_SPEED (1.0f)

/*
* GameClearLogo
* �Q�[���N���A�̃��S���o��
* �K���ȏ������Ƃ�����傫���Ƃ��܂�
*/

class GameClearLogo : public PlainSquarePolygon
{
public:
	GameClearLogo(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera);
	void update();
};