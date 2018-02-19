#pragma once

#include "polygonsquare.h"

// 0,0の原点を基準にして拡縮を行うクラス．移動しても大丈夫だよ

class ScalableObject : public SquarePolygonBase
{
private:
	POLSIZE zoom_level; // 拡大倍率
	void generate_vertexes();
public:
	ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 *_camera, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
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