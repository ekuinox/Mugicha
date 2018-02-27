#pragma once

#include "plain_square_polygon.h"

// 0,0�̌��_����ɂ��Ċg�k���s���N���X�D�ړ����Ă����v����

class ScalableObject : public PlainSquarePolygon
{
protected:
	POLSIZE zoom_level; // �g��{��
	void generate_vertexes();
public:
	ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~ScalableObject();
	void update();
	SQUARE get_square();
	void zoom(POLSIZE _zoom_level);
};