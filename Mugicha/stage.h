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
	Player *player; // �v���C���̕ϐ�
	std::vector<Enemy*> enemies; // �G�̉ϒ��z��
	Stage::GameInfo info; // ���s�Ǘ��ƌ���
	std::chrono::system_clock::time_point latest_update; // �ŏI�X�V
	std::chrono::system_clock::time_point latest_draw; // �ŏI�`��
	D3DXVECTOR2 camera;
	enum class Sign { ZERO,	PLUS, MINUS} zoom_sign; // �g���Ԃ��k����Ԃ����ăA���ł�
	POLSIZE zoom_level_target; // �ǂ��܂Ŋg�k���邩�Ƃ����A��
	POLSIZE zoom_level;
	POLSIZE map_size; // �w�i�̃f�J���ɂȂ�܂�

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