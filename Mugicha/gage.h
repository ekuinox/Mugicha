#pragma once
#include "always_displayed_polygon.h"

#define GAGE_WIDTH (40)
#define GAGE_HEIGHT (120)
#define GAGE_RECOVERY_SPEED (0.0005f) // �Q�[�W�񕜑��x
#define GAGE_X (25)
#define GAGE_Y (70)

/*
* Gage
* �g�k�Ɏg���R�X�g�̃Q�[�W
*/

class Gage : public AlwaysDisplayedPolygon
{
public:
	Gage(LPDIRECT3DTEXTURE9 _tex);
	void update();
	float consume(); // ��񕪏���Ďc�ʂ�Ԃ��C�R�X�g������Ȃ��ꍇ�͕��̒l��Ԃ�
	bool can_consume();
#if defined(_DEBUG) || defined(_STAGE_DEBUG)
	void cure(); // �S�񕜂�����
#endif
private:
	const float cost = 1.0f; // ��x�ɏ����R�X�g
	const float cost_gage_max = 2.0f; // �R�X�g�̖��^�����
	const float cost_recovery_speed = GAGE_RECOVERY_SPEED; // �R�X�g�񕜑��x�i��ms�j
	float current_cost; // ���݂̃R�X�g
};

