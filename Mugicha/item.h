#pragma once
#include "scalable_object.h"
#include "player.h"
#include <map>

class Item : public ScalableObject
{
public:
	Item(float _x, float _y, float _w, float _h, LPDIRECT3DTEXTURE9 _tex, int _layer, D3DXVECTOR2 &_camera, std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &_polygons, float _u = 0.0f, float _v = 0.0f, float _uw = 1.0f, float _vh = 1.0f);
	void update();
	SQUARE get_square();
	void hold();
	bool hold(Player *_player);
	void release();
	bool is_held();
private:
	std::map<SquarePolygonBase::PolygonTypes, std::vector<SquarePolygonBase*>> &polygons; // “–‚½‚èŒ©‚é‚æ‚¤‚É‚Á‚Æ‚«‚Ü‚·
	bool held; // ƒvƒŒƒCƒ„‚É’Í‚Ü‚ê‚Ä‚¢‚é‚©
	bool on_ground;
	Player *player;
	void generate_vertexes();
};

