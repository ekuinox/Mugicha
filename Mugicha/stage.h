#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "polygons_register.h"


using polygon_vec = std::vector<SquarePolygonBase*>;

class Stage {
public:
	enum class Status {
		Prep,
		LoadError,
		Ready,
		Playing,
		Pause,
		Clear,
		Failed,
		Retire,
		End
	};
	using GameInfo = struct _GameInfo
	{
		int score;
		int stage_number;
		enum Stage::Status status;
		Player::DeadReason dead_reason;
		_GameInfo() {}
		_GameInfo(int _score, enum Stage::Status _status, int _stage_number) : score(_score), status(_status), stage_number(_stage_number), dead_reason(Player::DeadReason::ALIVE){}
	};
private:
	// vars
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<SquarePolygonBase::PolygonTypes, polygon_vec> polygons;
	StageBackground *background;
	Goal *goal;
	Player *player; // ƒvƒŒƒCƒ„‚Ì•Ï”
	std::vector<Enemy*> enemies; // “G‚Ì‰Â•Ï’·”z—ñ
	Stage::GameInfo info; // ‘±sŠÇ—‚ÆŒ‹‰Ê
	std::chrono::system_clock::time_point latest_update; // ÅIXV
	std::chrono::system_clock::time_point latest_draw; // ÅI•`‰æ
	D3DXVECTOR2 camera;
	enum class Sign { ZERO,	PLUS, MINUS} zoom_sign; // Šg‘åó‘Ô‚©k¬ó‘Ô‚©‚Á‚ÄƒAƒŒ‚Å‚·
	POLSIZE zoom_level_target; // ‚Ç‚±‚Ü‚ÅŠgk‚·‚é‚©‚Æ‚¢‚¤ƒAƒŒ
	POLSIZE zoom_level;
	POLSIZE map_size; // ”wŒi‚ÌƒfƒJ‚³‚É‚È‚è‚Ü‚·

	bool switch_sample;

	// funcs
	void multi_texture_loader(std::map<std::string, const char *> _textures);
	void multi_texture_loader(const char *filepath);
	bool stagefile_loader(const char* filepath);
	void init();
	void update();
	void draw();

	template<typename _T>
	_T emplace_polygon_back(SquarePolygonBase::PolygonTypes type, _T polygon);
public:
	Stage(char _stage_select);
	~Stage();

	GameInfo exec();
};