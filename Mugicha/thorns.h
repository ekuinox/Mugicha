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
public:
	enum class Vec { UP, DOWN, LEFT, RIGHT };
private:
	SquarePolygonBase *floor; // RaggedFloor�ł���K�v�͂Ȃ��i�H�j
	Player *player;
	enum Vec vec;
	
public:
	Thorns(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, Player *_player, enum Vec _vec, SquarePolygonBase *_floor = nullptr, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void set_floor(std::vector<SquarePolygonBase*> _floors); // floor��nullptr�̂܂܂̏ꍇ�����Őݒ肵�Ȃ����܂��D
	void set_floor(SquarePolygonBase *_floor); // floor��nullptr�̂܂܂̏ꍇ�����Őݒ肵�Ȃ����܂��D
	void init();
	void update();
};