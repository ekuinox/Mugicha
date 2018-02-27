#pragma once
#include "plain_square_polygon.h"

/*
* StageBackground
* �X�e�[�W�̔w�i�ɂ̂ݎg��
* x,y,w,h�̕ύX�͍s��Ȃ�
* UV�l�Ŕw�i���X�N���[�������Ă����D
*/

class StageBackground : public PlainSquarePolygon
{
public:
	StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera);
	~StageBackground();
	void update();
};

