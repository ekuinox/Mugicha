#pragma once

#include "plain_square_polygon.h"
#include "player.h"
#include "scalable_object.h"
#include "background.h"
#include "enemy.h"

// SquarePolygonBase����h���������N���X���m�̓����蔻���Z�߂܂�
// ��{�I��SquarePolygonBase*��vector�Ŏ����Ă�̂ŁC�g�����̓L���X�g����K�v������D�܂��C�ʂɂ��̂܂܂���Ă����͂Ȃ��Ǝv�����ǁD
// �Ƃ����������蔻��̂Ƃ�����C�}�C�`�킩���Ă����̂ŁC������

// �Ȃ�Ń|�C���^�Ȃ񂾁H�H�H�H�H

// ��� : ���
bool is_collision(SquarePolygonBase *_a, SquarePolygonBase *_b);

// ��� :�@�g�k�|���S��
bool is_collision(SquarePolygonBase * _a, ScalableObject * _b);

// coords��size������

enum HitLine {
	NONE = 0x00,
	TOP = 0x01,
	BOTTOM = 0x02,
	LEFT = 0x03,
	RIGHT = 0x04
};

bool is_collision(const PlainSquarePolygon _a, const PlainSquarePolygon _b);

enum HitLine where_collision(SquarePolygonBase *_a, SquarePolygonBase *_b);