#pragma once
#include "square_polygon_base.h"

/*
* AlwaysDisplayedPolygon
* Camera‚É‰e‹¿‚³‚ê‚È‚¢ƒ|ƒŠƒSƒ“‚ðì‚é
*
*/

class AlwaysDisplayedPolygon : public SquarePolygonBase
{
public:
	AlwaysDisplayedPolygon(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~AlwaysDisplayedPolygon();

	void init();
	void update();
	void draw();
	bool is_drawing();
	void show();
	void hide();
	bool is_active();
	void enable();
	void disable();

	void on();
	void off();

	void change_texture(LPDIRECT3DTEXTURE9 _tex);

	SQUARE get_square();
	D3DXVECTOR2 get_coords();
	POLSIZE get_size();
	VERTEX_2D *get_vertexes();

	void add_coord(float _x, float _y);

	void zoom(POLSIZE _zoom_level);

	AlwaysDisplayedPolygon* operator->();
protected:
	void generate_vertexes();
};

