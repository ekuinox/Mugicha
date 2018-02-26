#pragma once

#ifdef _DEBUG_
#define _PLAYER_DEBUG
#endif

#include "plain_square_polygon.h"
#include "conf.h"
#include <map>

// �v���C���[�N���X

class Player : public PlainSquarePolygon
{
private:
	bool alive; // ����
	bool jumping; // �W�����v���Ă���
	bool ground; // ���n���
	bool moving; // �����Ă���(x��)
	DWORD jumped_at;
	bool controll_lock; // ������󂯕t���Ȃ�����
	D3DXVECTOR2 when_locked_coords; // ���b�N���ꂽ�Ƃ��̍��W�l
	POLSIZE zoom_level; // �Y�[���T�C�Y
	POLSIZE before_zoom_level; // �O�̃��c
	std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &polygons; // �����茩��悤�Ɏ����Ƃ��܂�
public:
	Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 *_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, int _layer, float _x, float _y, float _w, float _h, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Player();
	
	void init();
	void update();
	void zoom(POLSIZE _zoom_level);

	// �Ǝ��֐�
	bool jump();
	void lock();
	void unlock();
	void kill();
	bool dead();
};