#pragma once

#include "polygonsquare.h"
#include "player.h"
#include "scalable_object.h"


// SquarePolygonBase����h���������N���X���m�̓����蔻���Z�߂܂�
// ��{�I��SquarePolygonBase*��vector�Ŏ����Ă�̂ŁC�g�����̓L���X�g����K�v������D�܂��C�ʂɂ��̂܂܂���Ă����͂Ȃ��Ǝv�����ǁD
// �Ƃ����������蔻��̂Ƃ�����C�}�C�`�킩���Ă����̂ŁC������

// ��� : ���
bool is_collision(SquarePolygonBase* _a, SquarePolygonBase* _b);

// ��� :�@�g�k�|���S��
bool is_collision(SquarePolygonBase * _a, ScalableObject * _b);