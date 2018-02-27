#pragma once
#include "plain_square_polygon.h"

/*
* StageBackground
* �X�e�[�W�̔w�i�ɂ̂ݎg��
* x,y,w,h�̕ύX�͍s��Ȃ�
* UV�l�Ŕw�i���X�N���[�������Ă����D
* ���ƈႢ�C�����UV�l��XY�Ɠ����悤�ɒ��S�_���狁�߂�
*/

class StageBackground : public PlainSquarePolygon
{
public:
	using UVSet = struct _UVSet {
		float u, v, uw, vh;
		_UVSet() {}
		_UVSet(float _u, float _v, float _uw, float _vh)
			: u(_u), v(_v), uw(_uw), vh(_vh) {}
	};
	StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera);
	StageBackground(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, UVSet _uvset);
	~StageBackground();
	void update(); 
private:
	void generate_vertexes();
	POLSIZE zoom_level;
	UVSet uvset;
};

