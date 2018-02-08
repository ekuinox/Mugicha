#pragma once

#include "polygonsquare.h"

class ScalableObject : public SquarePolygonBase
{
private:
	float aspect_ratio; // ‘½•ª
	void generate_vertexes();
public:
	ScalableObject(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~ScalableObject();
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