#pragma once

#include "scalable_object.h"
#include "player.h"
#include "ragged_floor.h"

/*
** Thorns: �g�Q�N���X
** �v���C���ɓ�����Ǝ���
** �g�Q�ɂ͌���������C�������̂��͈̂�̑���u���b�N�Ȃǂɒ���t���Ă���
** 
*/

class Thorns : public ScalableObject
{
private:
	SquarePolygonBase *floor; // RaggedFloor�ł���K�v�͂Ȃ��i�H�j
	Player *player;
	bool up; // �����CTRUE�ŏ�����CFALSE�ŉ�����

public:
	Thorns();
	Thorns(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, SquarePolygonBase *_floor, Player *_player, bool _up, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void init();
	void update();
};