#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "polygons_register.h"
#include "helpers.h"

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
	// �ϐ��Q
	std::map<std::string, LPDIRECT3DTEXTURE9> textures; // �e�N�X�`���i�[
	D3DXVECTOR2 camera; // �J�����ʒu
	
	PolygonsContainer polygons; // ���ׂẴ|���S���������ɂ���
	Goal *goal; // �S�[��
	Player *player; // �v���C��
	Gage* gage; // �g�k�̃Q�[�W

	Stage::GameInfo info; // ���s�Ǘ��ƌ���
	
	time_point latest_update; // �ŏI�X�V
	time_point latest_draw; // �ŏI�`��
	
	enum class Sign { ZERO,	PLUS, MINUS} zoom_sign; // �g���Ԃ��k����Ԃ����ăA���ł�
	float zoom_level_target; // �ǂ��܂Ŋg�k���邩�Ƃ����A��
	float zoom_level; // �ŐV�̃Y�[�����x��

	POLSIZE map_size; // �w�i�̃f�J���ɂȂ�܂�
	
	std::map<std::string, bool> switches; // �X�C�b�`�}�b�v

	// �֐��Q
	void multi_texture_loader(std::map<std::string, const char *> _textures); // �e�N�X�`����ǂݍ���
	void multi_texture_loader(const char *filepath); // �e�N�X�`����ǂݍ��ނ��߂�map���쐬���ēn��
	bool stagefile_loader(const char* filepath); // �X�e�[�W�t�@�C�������[�h����
	void init(); // �X�e�[�W�̏����ݒ�����
	void update();
	void draw();

	template<typename _T>
	_T emplace_polygon_back(SquarePolygonBase::PolygonTypes type, _T polygon); // polygons�̎w�肳�ꂽ���x���̍Ō�ɂŒǉ�����
public:
	Stage(char _stage_select);
	~Stage();

	GameInfo exec(); // �O����͂��ꂵ���@���Ȃ�
};