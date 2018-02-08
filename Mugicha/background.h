#pragma once

#include "polygonsquare.h"
#include "conf.h"

class Background : public SquarePolygonBase {
private:
	void generate_vertexes();
public:
	Background(LPDIRECT3DTEXTURE9 _tex, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	~Background();
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