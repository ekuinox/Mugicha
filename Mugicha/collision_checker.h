#pragma once

#include "plain_square_polygon.h"
#include "player.h"
#include "scalable_object.h"
#include "background.h"
#include "enemy.h"

enum HitLine {
	NONE = 0x00,
	TOP = 0x01,
	BOTTOM = 0x02,
	LEFT = 0x04,
	RIGHT = 0x08
};

// SquarePolygonBase����h���������N���X���m�̓����蔻���Z�߂܂�
// ��{�I��SquarePolygonBase*��vector�Ŏ����Ă�̂ŁC�g�����̓L���X�g����K�v������D�܂��C�ʂɂ��̂܂܂���Ă����͂Ȃ��Ǝv�����ǁD
// �Ƃ����������蔻��̂Ƃ�����C�}�C�`�킩���Ă����̂ŁC������

bool is_collision(float a_x1, float a_x2, float b_x1, float b_x2, float a_y1, float a_y2, float b_y1, float b_y2);

// ��� : ���
bool is_collision(SquarePolygonBase *_self, SquarePolygonBase *_another);

// SQUARE vs SQUARE
bool is_collision(SQUARE _self, SQUARE _another);

// coords��size������
bool is_collisionA(SquarePolygonBase *_self, SquarePolygonBase *_another);

// �オ����������������
bool hit_top(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// ��������������������
bool hit_bottom(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// �E������������������
bool hit_right(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// ��������������������
bool hit_left(SQUARE &_self, SQUARE &_another, float sugar = 0.0f);

// self�ɊÂ���ǉ�����
enum HitLine where_collision(SquarePolygonBase *_self, SquarePolygonBase *_another, float sugar = 0.0f, bool without_precheck = true);

// �ǂ��ɓ������������E���܂� _a�̂ǂ̕ӂɑ΂��Ă�
enum HitLine where_collision(SquarePolygonBase *_self, SquarePolygonBase *_another);

// �v���C����SPB��
enum HitLine where_collision(Player *_self, SquarePolygonBase *_another);