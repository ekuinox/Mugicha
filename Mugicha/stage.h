#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "plain_square_polygon.h"
#include "player.h"
#include "background.h"
#include "scalable_object.h"
#include "enemy.h"
#include "ragged_floor.h"
#include "thorns.h"

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
	enum status status;
	_GameInfo() {}
	_GameInfo(int _score, enum status _status) : score(_score), status(_status) {}
};

enum Sign {
	ZERO,
	PLUS,
	MINUS,
};

class Stage {
private:
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<enum PolygonTypes, polygon_vec> polygons;
	Background *background;
	Player *player; // �v���C���̕ϐ�
	std::vector<Enemy*> enemies; // �G�̉ϒ��z��

	GameInfo info; // ���s�Ǘ��ƌ���
	DWORD latest_update; // �ŏI�X�V
	DWORD latest_draw; // �ŏI�`��
	DWORD elapsed_time; // �Q�[�����̎��ԁC�c�莞�Ԃ̕\���Ȃǂɗp��
	char stage_select; // �X�e�[�W�I��ԍ�
	D3DXVECTOR2 camera;
	enum Sign zoom_sign; // �g���Ԃ��k����Ԃ����ăA���ł�
	POLSIZE zoom_level_target; // �ǂ��܂Ŋg�k���邩�Ƃ����A��
	POLSIZE zoom_level;
	void multi_texture_loader(std::map<std::string, const char *> _textures);
	void init();
	void init1();
	void init2();
	void init3();
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