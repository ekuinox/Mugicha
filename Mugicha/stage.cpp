#include "stage.h"
#include "collision_checker.h"
#include "csv_loader.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>

// �R���X�g���N�^
Stage::Stage()
{
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	info.status = prep;

	init();
}

Stage::Stage(char _stage_select)
	: latest_update(timeGetTime()), latest_draw(timeGetTime()), info(0, prep, _stage_select)
{
	init();
}

// �f�X�g���N�^
Stage::~Stage()
{
	for (auto tex : textures)
	{
		if (tex.second)
		{
			tex.second->Release();
			tex.second = NULL;
		}
	}
}

// ������@���ăX�e�[�W���X�V������`�悵���肷��
GameInfo Stage::exec()
{
	update();
	draw();

	// ���낢��ȑ��s���������

	// �v���C���������Ă��邩�ŃQ�[���̑��s�𔻒�
	if (player->dead())
	{
		info.status = failed;
#ifdef _DEBUG
		printf("�A�J������ł����I\n");
#endif
	}

	// �v���C�����S�[�����Ă��邩
	if (goal->is_completed())
	{
		info.status = clear;
#ifdef _DEBUG
		printf("�Q�[���N���A�[�I\n");
#endif
	}

	return info;
}

void Stage::multi_texture_loader(std::map<std::string, const char *> _textures)
{
	for (auto _texture : _textures)
	{
		D3DXCreateTextureFromFile(d3d_device, _texture.second, &textures[_texture.first]); // ��ēǂݍ���
	}
}

void Stage::multi_texture_loader(const char * filepath)
{
	std::vector<std::vector<std::string>> table;

	if (!(csv_loader(filepath, table))) return;

	for (const auto& record : table)
	{
		if (record.size() == 2)
		{
			char texture_file[256];
			sprintf(texture_file, "%s%s", TEXTURES_DIR, record[1].c_str());
			if (FAILED(D3DXCreateTextureFromFile(d3d_device, texture_file, &textures[record[0]])))
			{
#ifdef _DEBUG
				printf("Failed to load texture file: %s\n", record[1].c_str());
#endif
			}
		}
	}

}

void Stage::stagefile_loader(const char * filepath)
{
	// TODO: �X�e�[�W�t�@�C������̃��[�_�����
	
	std::vector<std::vector<std::string>> table;

	if (!(csv_loader(filepath, table))) return; // csv�̓ǂݍ��݂Ɏ��s
	if (table[0].size() != 4) return; // ��s�ڂɂ�w,h�ɂ��Ă����ė~�����̂�

	// ���ׂĖ��ɂ���@��̂��Ƃ͍l���ĂȂ��@���݂܂���
	std::map<enum PolygonTypes, polygon_vec>().swap(polygons);
	
	// �}�b�v�̃T�C�Y����ꂽ��
	map_size = POLSIZE(std::atoi(table[0][0].c_str()) * CELL_WIDTH, std::atoi(table[0][1].c_str()) * CELL_HEIGHT);
	
	// �w�i�̓o�^
	(background = push_polygon_back(BACKGROUND, new Background(map_size.w / 2, map_size.h / 2, map_size.w, map_size.h, textures["BACKGROUND"], &camera)))->on();

	// �v���C���̓o�^
	// �v���C���͐�ɓo�^���Ă����Ȃ��ƌ�X���邢�ł�
	(player = push_polygon_back(PLAYER, REGISTER_PLAYER(std::atoi(table[0][2].c_str()) * CELL_WIDTH - CELL_WIDTH / 2, std::atoi(table[0][3].c_str()) * CELL_HEIGHT - CELL_HEIGHT / 2, textures["PLAYER"], &camera, polygons)))->on();

	for (auto i = map_size.h / CELL_HEIGHT; i >= 1; --i) // �P�c�����ԏ�܂�
	{
		for (auto j = 0; j < map_size.w / CELL_WIDTH; ++j) // �]�v�ȗv�f�܂œǂݍ��܂Ȃ��悤��
		{
			// �����œo�^���L���Ă���
			auto num = std::atoi(table[i][j].c_str());
			
			// ��邱��
			// x, y���v�Z���ďo���C�}�N���ďo���Ń|���S����o�^����D

			switch (num)
			{
			case 0:
				break;
			case 1:
				// �����Ńv���C��������������̂͂�߂܂��D
			//	(player = push_polygon_back(PLAYER, REGISTER_PLAYER(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["PLAYER"], &camera, polygons)))->on();
				break;
			case 2:
				break;
			case 3:
				(goal = push_polygon_back(GOAL, REGISTER_GOAL(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player)))->on();
				break;
			case 4:
				push_polygon_back(SCALABLE_OBJECT, REGISTER_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["SAMPLE1"], &camera))->on();
			//	push_polygon_back(RAGGED_FLOOR, REGISTER_RAGGED_FLOOR(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["BLOCK2"], &camera, player))->on();
				break;
			case 5:
			//	push_polygon_back(SCALABLE_OBJECT, REGISTER_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["SAMPLE1"], &camera))->on();
				push_polygon_back(RAGGED_FLOOR, REGISTER_RAGGED_FLOOR(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["BLOCK2"], &camera, player))->on();
				break;
			case 6:
				break;
			case 7:
				break;
			case 11:
				push_polygon_back(THORNS, REGISTER_THORNS_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 12:
				push_polygon_back(THORNS, REGISTER_THORNS_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 13:
				push_polygon_back(THORNS, REGISTER_THORNS_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 14:
				push_polygon_back(THORNS, REGISTER_THORNS_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ORIGIN"], &camera, player))->on();
				break;
			case 15:
				break;
			case 16:
				break;
			case 21:
				break;
			case 22:
				break;
			case 23:
				break;
			case 24:
				break;
			case 31:
				break;
			case 32:
				break;
			case 33:
				break;
			}
		}
	}

	// �g�Q�̏����Z�b�g���Ă����܂�
	std::vector<SquarePolygonBase*> floors;
	for (const auto& type : { SCALABLE_OBJECT, ENEMY, RAGGED_FLOOR}) floors.insert(floors.end(), polygons[type].begin(), polygons[type].end());
	for (const auto& thorns : polygons[THORNS]) static_cast<Thorns*>(thorns)->set_floor(floors);

#ifdef _DEBUG

	// ���_
	polygons[DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, &camera));
	polygons[DEBUG_GUIDE].back()->on();

	// x���K�C�h���C��
	polygons[DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, &camera));
	polygons[DEBUG_GUIDE].back()->on();

	// y���K�C�h���C��
	polygons[DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, &camera));
	polygons[DEBUG_GUIDE].back()->on();

#endif
}

// ������, �R���X�g���N�^����Ă�
void Stage::init()
{
	char filepath[256]; // �t�@�C���p�X�i�[

	sprintf(filepath, STAGEFILES_DIR "textures_%02d.csv", info.stage_number);
	multi_texture_loader(filepath); // �e�N�X�`���̃��X�g��ǂݍ��� => ��������

	sprintf(filepath, STAGEFILES_DIR "stage_%02d.csv", info.stage_number);
	stagefile_loader(filepath); // �|���S���t�@�C���p�X���w�肵�ēǂݍ���

	zoom_level = { 1, 1 };
	zoom_sign = ZERO;
}

// �X�V����
void Stage::update()
{
	// ���Ԃ��C�ɂ��Ȃ�����

	// �^�C�g���ɖ߂�i���m�F�j
	if (GetKeyboardTrigger(DIK_1))
	{
		info.status = retire;
		return;
	}

	// �g�k
	if (zoom_sign == ZERO)
	{
		if (GetKeyboardTrigger(DIK_O) && zoom_level.h < 2.0f) // �g��
		{
			zoom_sign = PLUS;
			zoom_level_target.w = zoom_level.w * 2;
			zoom_level_target.h = zoom_level.h * 2;
			player->lock();
		}
		if (GetKeyboardTrigger(DIK_L) && zoom_level.w > 0.5f) // �k��
		{
			zoom_sign = MINUS;
			zoom_level_target.w = zoom_level.w / 2;
			zoom_level_target.h = zoom_level.h / 2;
			player->lock();
		}
	}
	
	// �v���C�����W�����v������
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		player->jump();
	}


	// ���Ԃ��C�ɂ������
	auto current = timeGetTime();
	if (current - latest_update < 1) return;
	latest_update = current;

	if (zoom_sign == PLUS)
	{
		if (zoom_level.w < zoom_level_target.w)
		{
			zoom_level.w *= 1.001f;
			zoom_level.h *= 1.001f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = ZERO;
			player->unlock();
		}
	}

	if (zoom_sign == MINUS)
	{
		if (zoom_level.w > zoom_level_target.w)
		{
			zoom_level.w /= 1.001f;
			zoom_level.h /= 1.001f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = ZERO;
			player->unlock();
		}
	}

	// �S�Y�[���ύX
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->zoom(zoom_level);
	}

	// ��������X�V����
	camera.x = player->get_coords().x;
	camera.y = player->get_coords().y + 200; // �v���C������̃J�����̍����C��������Ȃ񂩕ς��Ǝv��
	
	// ��ʊO�͌����Ȃ��悤�ɂ���
	unless(camera.x < map_size.w * zoom_level.w - SCREEN_WIDTH / 2) camera.x = map_size.w * zoom_level.w - SCREEN_WIDTH / 2;
	unless (camera.y < map_size.h * zoom_level.h - SCREEN_HEIGHT / 2) camera.y = map_size.h * zoom_level.h - SCREEN_HEIGHT / 2;
	if (camera.x < SCREEN_WIDTH / 2) camera.x = SCREEN_WIDTH / 2;
	if (camera.y < SCREEN_HEIGHT / 2) camera.y = SCREEN_HEIGHT / 2;

	// �|���S���̑S�X�V
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->update();
	}
}

// �`�揈��
void Stage::draw()
{
	auto current = timeGetTime();
	if (current - latest_draw < 1000 / 60) return;
	latest_draw = current;

	// ��������`�揈��

	// �\�[�g
	polygon_vec drawing_polygons;
	for (const auto& pol_vec : polygons)
	{
		for (const auto& polygon : pol_vec.second)
		{
			drawing_polygons.push_back(polygon);
		}
	}

	// �傫�����̂��O�ɗ���悤��
	sort(drawing_polygons.begin(), drawing_polygons.end(), [](const SquarePolygonBase* x, const SquarePolygonBase* y) {return x->layer > y->layer; });
	
	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& _polygons : polygons)
		{
			for (const auto& drawing_polygon : drawing_polygons) drawing_polygon->draw();
		}
	}
	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}

// polygons�̎w�肵�����x���Ƀ|���S����ǉ�
template<typename _T>
_T Stage::push_polygon_back(PolygonTypes type, _T polygon)
{
	polygons[type].emplace_back(polygon);

	return polygon;
}
