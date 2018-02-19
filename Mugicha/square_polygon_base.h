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
using VERTEX_2D = struct
{
	float x, y, z;
	float rhw;
	D3DCOLOR diffuse;
	float u, v;
};

using POLSIZE = struct _POLSIZE
{
	float w;
	float h;
	_POLSIZE() {};
	_POLSIZE(float _w, float _h) : w(_w), h(_h) {};
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
	float direction; // �ړ�����
	float angle; // �{�̂̉�]�p�x
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
	
	virtual void change_texture(LPDIRECT3DTEXTURE9) = 0; // �e�N�X�`���ύX�Ɏg���܂�

	// Getter
	virtual D3DXVECTOR2 get_coords() = 0;
	virtual POLSIZE get_size() = 0;
	virtual VERTEX_2D *get_vertexes() = 0;

	virtual void add_coord(float _x, float _y) = 0; // x, y���W�ɉ��Z
	virtual void zoom(POLSIZE _zoom_level) = 0; // �{���ύX���s��
};

/* global variable */
extern LPDIRECT3DDEVICE9 d3d_device; // Devece obj