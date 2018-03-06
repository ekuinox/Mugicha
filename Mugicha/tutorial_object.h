#pragma once
#include "scalable_object.h"

#define TUTORIAL_TEXTURE_UV_W (0.25f)
#define TUTORIAL_TEXTURE_UV_H (0.125f)
#define TEXTURE_ANIMATION_CHANGE_INTERVAL (1000)
#define TUTORIALS_DEFAULT_LAYER (5)

/*
* TutorialObject
* �`���[�g���A���̃{�^�����A���R������
*/

class TutorialObject : public ScalableObject
{
public:
	using CELL = struct _CELL {
		// ���オ0,0
		// �|���Z��uv���o��
		int x, y;
		_CELL() : x(0), y(0){}
		_CELL(int _x, int _y) : x(_x), y(_y) {}
	};
	TutorialObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, CELL one, CELL two);
	void update();
private:
	CELL uvs[2]; // 2�̍��W�ŃA�j���[�V����������
	int uv_sel; // ��̂��߂̂���
};