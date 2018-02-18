#pragma once

#include "polygonsquare.h"
#include "conf.h"

class Background : public SquarePolygonBase {
private:
	float aspect_ratio;
	void generate_vertexes();
public:
	Background(LPDIRECT3DTEXTURE9 _tex, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Background();
	void update();
	void draw();
	bool is_drawing();
	void show();
	void hide();
	bool is_active();
	void enable();
	void disable();
	void change_texture(LPDIRECT3DTEXTURE9 _tex);
	bool is_collision(SquarePolygonBase *pol);
	D3DXVECTOR2 get_coords();
	POLSIZE get_size();
	void add_coord(float _x, float _y);
	void zoom(POLSIZE _zoom_level);
};