#pragma once
#include "always_displayed_polygon.h"

#define GAGE_WIDTH (100)
#define GAGE_HEIGHT (200)

/*
* Gage
* �g�k�Ɏg���R�X�g�̃Q�[�W
*/

class Gage : public AlwaysDisplayedPolygon
{
public:
	Gage(float _x, float _y, LPDIRECT3DTEXTURE9 _tex);
	void update();
	float consume(); // ��񕪏���Ďc�ʂ�Ԃ��C�R�X�g������Ȃ��ꍇ�͕��̒l��Ԃ�
	bool can_consume();
private:
	const float cost = 1.0f; // ��x�ɏ����R�X�g
	const float cost_gage_max = 2.0f; // �R�X�g�̖��^�����
	const float cost_recovery_speed = 0.01f; // �R�X�g�񕜑��x�i��ms�j
	float current_cost; // ���݂̃R�X�g
};

