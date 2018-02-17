#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "polygonsquare.h"
#include "player.h"
#include "background.h"
#include "scalable_object.h"

using polygon_vec = std::vector<SquarePolygonBase*>;

enum status {
	prep, // ������
	ready, // ��������
	playing, // �v���C��
	pause, // �|�[�Y
	clear, // �N���A
	failed, // ���s
	end, // �I��
};

class Stage {
private:
	std::map<std::string, LPDIRECT3DTEXTURE9> textures;
	std::map<std::string, polygon_vec> polygons;
	enum status status;
	DWORD latest_update; // �ŏI�X�V
	DWORD latest_draw; // �ŏI�`��
	DWORD elapsed_time; // �Q�[�����̎��ԁC�c�莞�Ԃ̕\���Ȃǂɗp��
	char stage_select; // �X�e�[�W�I��ԍ�
	D3DXVECTOR2 camera;

	void multi_texture_loader(std::map<std::string, const char *> _textures);
	void init();
	void update();
	void draw();
public:
	Stage();
	Stage(char _stage_select);
	~Stage();

	void exec();
};