#pragma once

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#include "d3dx9.h"
#include "input.h"
#include <cmath>
#include <vector>
#include <time.h>
#include "conf.h"

#ifdef _DEBUG
#include <iostream>
#include "debug_console.h"
#endif

// �h���[����{��
using VERTEX_2D = struct _VERTEX_2D
{
	float x, y, z;
	float rhw;
	D3DCOLOR diffuse;
	float u, v;
	_VERTEX_2D() {};
	_VERTEX_2D(float _x, float _y, float _u, float _v) : x(_x), y(_y), z(0.0f), u(_u), v(_v), rhw(1.0f), diffuse(D3DCOLOR_RGBA(255, 255, 255, 200)){};
};

using SQUARE = struct _SQUARE
{
	float x, y, w, h;
	_SQUARE() {};
	_SQUARE(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {};
};

using POLSIZE = struct _POLSIZE
{
	float w;
	float h;
	_POLSIZE() {};
	_POLSIZE(float _w, float _h) : w(_w), h(_h) {};
};

// �|���S���̎�ʂ𔻒肳����A���C�h���𑝂₹�΂����ɏ����Ă��Ȃ��Ƃ���ǂ�
enum PolygonTypes
{
	BACKGROUND,
	PLAYER,
	ENEMY,
	SCALABLE_OBJECT,
	PLAIN,
};

/*
* SquarePolygonBase
* �l�p�`��Polygon���Ǘ��`�悷�钊�ۃN���X
*/

class SquarePolygonBase
{
protected:
	// === �ϐ� ===
	DWORD latest_update;
	float x, y, w, h; // x, y => ���S���W
	D3DXVECTOR2 drawing_coord; // �`��p�̍��W
	float u, v, uw, vh; // u,v => ����Cuv, vh => ������
	float speed; // �ړ����x
	bool drawing; // �`�悷�邩�ǂ����̃t���O
	bool status; // �L��/���� => update�p��
	LPDIRECT3DTEXTURE9 tex; // �e�N�X�`��
	VERTEX_2D vertexes[4]; // �|���S�����_��� => ���ۃh���[����Ƃ��ɐ������Ďg���܂�

	D3DXVECTOR2 *camera; // �`�撆�S���W => ��{�I�Ƀv���C�����S

	// === �֐� ===
	virtual void generate_vertexes() = 0; // vertexes�𐶐�����̂Ɏg���Ddraw()���Ă΂��ׂ��ŁCpublic�ɂ͂��Ă܂���

public:
	// === �ϐ� ===
	int layer; // ���C���[�ԍ� �d���͉C�������傫�����̂���`�悵����

	// === �֐� ===

	SquarePolygonBase() {}
	SquarePolygonBase(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, float _u, float _v, float _uw, float _vh)
		: x(_x), y(_y), w(_w), h(_h), tex(_tex), layer(_layer), camera(_camera), u(_u), v(_v), uw(_uw), vh(_vh), drawing(false), status(false), drawing_coord(_x, _y), speed(1.0f), latest_update(timeGetTime()) {}

	virtual void init() = 0; // ����������
	
	// Update����
	virtual void update() = 0; // �X�V���s��
	virtual bool is_active() = 0; // �X�V�����邩�̃t���O���擾
	virtual void enable() = 0; // �X�V������
	virtual void disable() = 0; // �X�V���Ȃ�����
	
	// drawing�t���O�Ɋւ���
	virtual void draw() = 0; // �`����s��
	virtual bool is_drawing() = 0; // �`�悳���邩�̃t���O���擾
	virtual void show() = 0; // �`�悷��
	virtual void hide() = 0; // �`�悵�Ȃ�����
	
	virtual void on() = 0; // status, drawing�𗼕�true�ɂ���
	virtual void off() = 0; // status, drawing�𗼕�false�ɂ���


	virtual void change_texture(LPDIRECT3DTEXTURE9) = 0; // �e�N�X�`���ύX�Ɏg���܂�

	// Getter
	virtual SQUARE get_square() = 0;
	virtual D3DXVECTOR2 get_coords() = 0;
	virtual POLSIZE get_size() = 0;
	virtual VERTEX_2D *get_vertexes() = 0;

	virtual void add_coord(float _x, float _y) = 0; // x, y���W�ɉ��Z
	virtual void zoom(POLSIZE _zoom_level) = 0; // �{���ύX���s��
};

/* global variable */
extern LPDIRECT3DDEVICE9 d3d_device; // Devece obj