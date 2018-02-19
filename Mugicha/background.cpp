#include "background.h"

Background::Background(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera, float _u, float _v, float _uw, float _vh)
{
	x = BACKGROUND_X;
	y = BACKGROUND_Y;
	w = BACKGROUND_WIDTH;
	h = BACKGROUND_HEIGHT;
	tex = _tex;
	// ������uv�󂯎���悤�ɂ��Ă��邯�ǁC���ێg���Ƃ���uv�͂����N���X���ł�����悤�ɂ��Ă����Ǝv��
	u = _u;
	v = _v;
	uw = _uw;
	vh = _vh;
	layer = INT_MAX;
	camera = _camera;
	zoom_level = { 1, 1 };
}

Background::~Background()
{
	// ��
}

void Background::update()
{
	if (!status) return;
	
	DWORD current = timeGetTime();

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{
		// uv�l�̕ύX�Ȃǂ�����

		latest_update = current;
	}
}