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

/*
* �l�p�`��Polygon���Ǘ��`�悷��N���X
* SquarePolygonBase => ����
* PlainSquarePolygon => SquarePolygonBase���p�������V���v���Ȏl�p�`�p�̃N���X
*/

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
	// Update & Draw
	virtual void update() = 0;
	virtual void draw() = 0; // drawing���Ă���draw
							 // drawing�t���O�Ɋւ���
	virtual bool is_drawing() = 0; // return drawing;
	virtual void show() = 0; // �`�悷��
	virtual void hide() = 0; // �`�悵�Ȃ�����
	virtual void change_texture(LPDIRECT3DTEXTURE9) = 0; // �e�N�X�`���ύX�Ɏg���܂�

	virtual bool is_active() = 0; // return status;
	virtual void enable() = 0; // �X�V������
	virtual void disable() = 0; // �X�V���Ȃ�����

	virtual D3DXVECTOR2 get_coords() = 0;
	virtual POLSIZE get_size() = 0;
	virtual VERTEX_2D *get_vertexes() = 0;

	virtual void add_coord(float _x, float _y) = 0; // x, y���W�ɉ��Z

	virtual void zoom(POLSIZE _zoom_level) = 0; // �{���ύX���s��
};

class PlainSquarePolygon : public SquarePolygonBase
{
private:
	void generate_vertexes();
public:
	PlainSquarePolygon();
	PlainSquarePolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~PlainSquarePolygon();
	void update();
	void draw();
	bool is_drawing();
	void show();
	void hide();
	bool is_active();
	void enable();
	void disable();
	void change_texture(LPDIRECT3DTEXTURE9 _tex);

	D3DXVECTOR2 get_coords();
	POLSIZE get_size();
	VERTEX_2D *get_vertexes();

	void add_coord(float _x, float _y);

	void zoom(POLSIZE _zoom_level);
};

/* global variable */
extern LPDIRECT3DDEVICE9 d3d_device; // Devece obj