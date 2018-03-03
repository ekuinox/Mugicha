#pragma once
#include "scalable_object.h"

/*
* GimmickFloor
* �X�C�b�`�������ꂽ�Ƃ��ɕ\������u���b�N
*/

class GimmickFloor : public ScalableObject
{
public:
	GimmickFloor(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, bool &_appearing, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
	bool is_appearing();
private:
	bool &appearing;
};