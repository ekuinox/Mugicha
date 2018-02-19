#pragma once

#include "polygonsquare.h"

// �ړ����Ȃ��O��ō���Ă�
// �g�k�ɔ����ړ��͍s��
// �悭�킩���
class ScalableObject : public SquarePolygonBase
{
private:
	POLSIZE zoom_level; // �g��{��
	D3DXVECTOR2 scaling_base; // �g�傷�邽�߂̊�D�R���X�g���N�g���Ɍ��߂�
	char scaling_dir; // �g�傷����� 0 => ����, 1 => �E��, 2 => �E��, 3 => ����
	void generate_vertexes();
public:
	ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, char _scaling_dir, int _layer, D3DXVECTOR2 *_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~ScalableObject();
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
	void add_coord(float _x, float _y);
	void zoom(POLSIZE _zoom_level);
	VERTEX_2D *get_vertexes();
};