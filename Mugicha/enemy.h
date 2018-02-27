#pragma once

#include "scalable_object.h"
#include "player.h"

// �g�k�����Ⴄ�G����p�̃N���X�ł�
// ���܂͂܂������Ȃ�

class Enemy : public ScalableObject
{
public:
	enum class Vec{ LEFT, RIGHT}; // �i�ތ���
private:
	bool alive; // ����
	bool on_ground; // �ڒn
	bool jumping; // �W�����v���Ă���
	bool moving; // �s��
	DWORD jumped_at; // �W�����v�������_�̎���
	Vec vec;
	Player *player;
	std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &polygons;
public:
	Enemy(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, int _layer, float _x, float _y, float _w, float _h, Vec _vec, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
};