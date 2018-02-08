#pragma once
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#include "d3dx9.h"
#include "input.h"
#include <cmath>
#include <vector>
#include <time.h>
#include "conf.h"

/*
* �l�p�`��Polygon���Ǘ��`�悷��N���X
* SquarePolygonBase => ����
* PlainSquarePolygon => SquarePolygonBase���p�������V���v���Ȏl�p�`�p�̃N���X
*/

// �h���[����{��
typedef struct
{
	float x, y, z;
	float rhw;
	D3DCOLOR diffuse;
	float u, v;
} VERTEX_2D;

/*
* NOTES
* protected�ȕϐ���������public�Ȋ֐�������Ƃ��Ȃ��ᑽ���l��
*/

class SquarePolygonBase // ���ۃN���X
{
private:
	// �����Ȃ�
protected:
	// === �ϐ� ===
	DWORD latest_update;
	float direction; // �ړ�����
	float angle; // �{�̂̉�]�p�x
	float u, v, uw, vh; // u,v => ����Cuv, vh => ������
	float speed; // �ړ����x
	bool drawing; // �`�悷�邩�ǂ����̃t���O
	bool status; // �L��/���� => update�p��
	LPDIRECT3DTEXTURE9 tex; // �e�N�X�`��
	VERTEX_2D vertexes[4]; // �|���S�����_��� => ���ۃh���[����Ƃ��ɐ������Ďg���܂�

						   // === �֐� ===
	virtual void generate_vertexes() = 0; // vertexes�𐶐�����̂Ɏg���Ddraw()���Ă΂��ׂ��ŁCpublic�ɂ͂��Ă܂���

public:
	// == �ϐ� ===
	float x, y, w, h; // x, y => ���S���W
	int priority; // �`��D��x => ���̂Ƃ���g���Ă��Ȃ�
	bool flags[3]; // �\���̃t���O�C�����Ɏg���鑽��
				   // === �֐� ===
	SquarePolygonBase();
	virtual ~SquarePolygonBase();

	// Update & Draw
	virtual void update() = 0;
	virtual void draw() = 0; // drawing���Ă���draw
							 // drawing�t���O�Ɋւ���
	virtual bool is_drawing() = 0; // return drawing;
	virtual void switch_drawing(bool) = 0; // �󂯎�����ϐ���drawing�t���O��؂�ւ���
	virtual void switch_drawing() = 0; // drawing = !drawing;
	virtual void change_texture(LPDIRECT3DTEXTURE9) = 0; // �e�N�X�`���ύX�Ɏg���܂�
	virtual bool is_collision(SquarePolygonBase*) = 0; // �|���S�����m�̓����蔻���

	virtual void switch_status(bool) = 0; // Update�p�̃t���O�C�X�V���邩�𔻒f����̂�
	virtual bool is_active() = 0; // return status;
};

class PlainSquarePolygon : public SquarePolygonBase
{
private:
	void generate_vertexes();
public:
	PlainSquarePolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~PlainSquarePolygon();
	void update();
	void draw();
	bool is_drawing();
	void switch_drawing(bool _drawing);
	void switch_drawing();
	void change_texture(LPDIRECT3DTEXTURE9 _tex);
	bool is_collision(SquarePolygonBase *pol);
	void switch_status(bool _status);
	bool is_active();
};

/* global variable */
extern LPDIRECT3DDEVICE9 d3d_device; // Devece obj