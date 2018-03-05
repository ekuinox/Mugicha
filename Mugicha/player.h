#pragma once

#include "plain_square_polygon.h"
#include "conf.h"
#include <map>
#include "input.h"
#include "XInputController.h"
#include "item.h"
#include "helpers.h"
#include "audio_controller.h"
#include <memory>

// �����l�Ȃ�
#define PLAYER_SPEED (0.675f) // �v���C��ړ����x
#define PLAYER_JUMP_POWER (2.0f) // �W�����v�́C1ms�ɂǂ񂾂���Ԃ�
#define PLAYER_JUMP_HEIGHT (CELL_HEIGHT * 2.5) // �W�����v���鍂��
#define PLAYER_HOLD_JUMP_HEIGHT (CELL_HEIGHT * 4.0) // �������W�����v���鍂��
#define PLAYER_FALLING (1.0f) // �������x
#define PLAYER_WIDTH (CELL_WIDTH * 0.8) // �v���C���̕�
#define PLAYER_HEIGHT (CELL_HEIGHT * 0.8) // �v���C���̍���
#define COLLISION_CHECK_POLYGONTYPES { \
	SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, \
	SquarePolygonBase::PolygonTypes::RAGGED_FLOOR, \
	SquarePolygonBase::PolygonTypes::THORN, \
	SquarePolygonBase::PolygonTypes::AIRCANNON, \
	SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH \
	} // �����蔻������|���S���̃^�C�v�܂Ƃ�
#define FALLING_OUT_Y (-10) // ��ʊO�����̍���
#define KNOCKBACK_VOLUME ((CELL_WIDTH + CELL_HEIGHT) / 2) // �m�b�N�o�b�N����

// �v���C���[�N���X

class Player : public PlainSquarePolygon
{
public:
	// ������
	enum class DeadReason
	{
		ALIVE, // �����Ƃ�킢
		Sandwiched, // ���܂�Ď���
		HitEnemy, // �G�ɐڐG
		HitThorn, // �g�Q�ɐڐG
		Falling, // ���ɗ���
		HitMagma, // �}�O�}�ŃA�`�`
		Shot, // Bullet�Ƀh�K�[��
		FallenHellGate, // HellGate(����)�Ɉ��ݍ��܂ꂽ
	};
	// ������
	enum class Vec
	{
		CENTER,
		RIGHT,
		LEFT
	};
private:
	PolygonsContainer &polygons; // �����茩��悤�Ɏ����Ƃ��܂�

	DeadReason dead_reason; // �����Ă��邩�C�܂�����ł���Ȃ炻�̗��R
	
	bool jumping; // �W�����v���Ă���
	float jumped_at;
	bool ground; // ���n���
	bool moving; // �����Ă���(x��)
	
	bool controll_lock; // ������󂯕t���Ȃ�����
	D3DXVECTOR2 when_locked_coords; // ���b�N���ꂽ�Ƃ��̍��W�l
	
	float zoom_level; // �Y�[���T�C�Y
	float before_zoom_level; // �O�̃��c

	Player::Vec vec;
	Player::Vec old_vec;
	
	Item *item; // �����Ă���A�C�e�����i�[����D�����Ă��Ȃ��ꍇ��nullptr�ɂ���D
	
	AudioController *audiocontroller;

	bool collision_for_enemies();
	bool collision_for_thorns();
	bool collision_for_magmas();
	bool collision_for_bullets();
	D3DXVECTOR2 collision_for_knockback_bullets();
	void collision_for_knockback_bullets(D3DXVECTOR2 &vector, char &result);
	bool is_fallen_hellgate();
	char collision_check();
	void sandwich_check(char &result); // ���܂�Ă邩
	void falling_out_check(char &result); // ��ʊO�ɂ܂ŗ������Ă��Ȃ���
	void ground_check(char &result); // ���n���Ă��邩
	void head_check(char &result); // ���ł��ĂȂ���
	void controlls(D3DXVECTOR2 &vector, char &result);
	void jump(D3DXVECTOR2 &vector, char &result);
	void drifting(D3DXVECTOR2 &vector); // ����������
	bool catch_item(); // �A�C�e��������
	void release_item(); // �A�C�e���𗣂�
	bool is_holding_item(); // �A�C�e���������Ă��邩

	void generate_vertexes();
public:
	Player(LPDIRECT3DTEXTURE9 _tex, D3DXVECTOR2 &_camera, PolygonsContainer &_polygons, int _layer, float _x, float _y, float _w = PLAYER_WIDTH, float _h = PLAYER_HEIGHT, float _u = 0.0f, float _v = 0.0f, float _uw = 0.1f, float _vh = 0.25f);
	~Player();
	void init();
	void update();
	void zoom(float _zoom_level);

	// �Ǝ��֐�
	
	void lock();
	void unlock();
	void kill(const DeadReason &_dead_reason);
	bool is_jumping();
	void trigger_controlls();
	Player::DeadReason dead();
	Player::Vec get_vec();
	SQUARE get_square();
};