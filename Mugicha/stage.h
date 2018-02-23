#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "polygons_register.h"


using polygon_vec = std::vector<SquarePolygonBase*>;

enum status {
	prep, // ������
	ready, // ��������
	playing, // �v���C��
	pause, // �|�[�Y
	clear, // �N���A
	failed, // ���s
	retire, // ���߂��ꍇ
	end, // �I��
};

using GameInfo = struct _GameInfo
{
	int score;
	int stage_number;
	enum status status;
	_GameInfo() {}
	_GameInfo(int _score, enum status _status, int _stage_number) : score(_score), status(_status), stage_number(_stage_number) {}
};

enum Sign {
	ZERO,
	PLUS,
	MINUS,
};

class Stage {
private:
	// vars
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<enum PolygonTypes, polygon_vec> polygons;
	Background *background;
	Goal *goal;
	Player *player; // �v���C���̕ϐ�
	std::vector<Enemy*> enemies; // �G�̉ϒ��z��
	GameInfo info; // ���s�Ǘ��ƌ���
	DWORD latest_update; // �ŏI�X�V
	DWORD latest_draw; // �ŏI�`��
	DWORD elapsed_time; // �Q�[�����̎��ԁC�c�莞�Ԃ̕\���Ȃǂɗp��
	D3DXVECTOR2 camera;
	enum Sign zoom_sign; // �g���Ԃ��k����Ԃ����ăA���ł�
	POLSIZE zoom_level_target; // �ǂ��܂Ŋg�k���邩�Ƃ����A��
	POLSIZE zoom_level;
	POLSIZE map_size; // �w�i�̃f�J���ɂȂ�܂�

	// funcs
	void multi_texture_loader(std::map<std::string, const char *> _textures);
	void multi_texture_loader(const char *filepath);
	void stagefile_loader(const char* filepath);
	void init();
	void update();
	void draw();

	template<typename _T>
	_T push_polygon_back(enum PolygonTypes type, _T polygon);
public:
	Stage();
	Stage(char _stage_select);
	~Stage();

	GameInfo exec();
};