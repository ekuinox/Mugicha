#pragma once
#include "scalable_object.h"

/*
*
* GimmickSwitch
* �K���ȃt���O���������āC���̃N���X�ŃI���ɂ����Ƃ��ɂ��̃t���O���I���ɂ��Ă��
* ������������
*/

class GimmickSwitch : public ScalableObject
{
public:
	GimmickSwitch(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, bool &_flag, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void press(); // on�ɂ���
	void release(); // off�ɂ���
private:
	bool &flag; // �X�C�b�`���L���ɂȂ����Ƃ��ɃI���ɂ���t���O�������ɂ���܂�
};