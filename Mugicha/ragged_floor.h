#pragma once

#include "scalable_object.h"
#include "player.h"

// �{�������̃N���X
// ��ɂ��΂炭����Ă���Ɨ�����Ƃ����A���D
// ������x���Ƃ������ɖ߂��܂�

class RaggedFloor : public ScalableObject
{
private:
	Player *player; // ��������݂邽�߂̃v���C��
	D3DXVECTOR2 init_pos; // �����ʒu
	bool burdened; // ��Ƀv���C��������Ă���Ƃ���TRUE
	float burdened_time; // burdened��TRUE�̊ԉ��Z����
	bool falling; // �������Ă���Ƃ��̃t���O
	float disapper_time; // �����Ă�������

	void init(); // �������Ɏg��
public:
	RaggedFloor();
	RaggedFloor(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, Player *_player, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
};