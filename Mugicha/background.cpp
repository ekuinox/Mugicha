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
	// ��
	if (!status) return;
	
	DWORD current = timeGetTime();

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{
		// uv�l�̕ύX�Ȃǂ�����

		/*
		// �v���C���̈ړ��ɍ��킹�Ĕw�i�����E�Ɉړ�������
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFTARROW)) // �������ւ̈ړ�
		{
			u -= 0.0001f;
		}
		else if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHTARROW)) // �E�����ւ̈ړ�
		{
			u += 0.0001f;
		}
		
		
		// �g�k
		if (GetKeyboardPress(DIK_NUMPAD8)) // �g��
		{
			w += 1.0f;
			h += 1.0f * aspect_ratio;
		}
		else if (GetKeyboardPress(DIK_NUMPAD2)) // �k��
		{
			w -= 1.0f;
			h -= 1.0f * aspect_ratio;
		}

		// ���E����
		if (w < SCREEN_WIDTH) w = SCREEN_WIDTH;
		if (h < SCREEN_HEIGHT) h = SCREEN_HEIGHT;
		*/

		latest_update = current;
	}
}

void Background::draw()
{
	if(!drawing) return;
	
	generate_vertexes();
	d3d_device->SetTexture(0, tex);
	d3d_device->SetFVF(FVF_VERTEX_2D);

	d3d_device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		sizeof(this->vertexes) / sizeof(VERTEX_2D) - 2, // �|���S����
		this->vertexes,
		sizeof(VERTEX_2D)
	);

#ifdef __DEBUG
	for (const auto vertex : vertexes)
	{
		std::cout << "(" << vertex.x << ", " << vertex.y << "),";
	}
	std::cout << std::endl;
#endif
}