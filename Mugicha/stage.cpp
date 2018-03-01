#include "stage.h"
#include "collision_checker.h"
#include "csv_loader.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>

// �R���X�g���N�^
Stage::Stage(char _stage_select)
	: latest_update(std::chrono::system_clock::now()), latest_draw(std::chrono::system_clock::now()), info(0, Stage::Status::Prep, _stage_select), switch_sample(false), zooming(false)
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
Stage::GameInfo Stage::exec()
{
	update();
	draw();

	// ���낢��ȑ��s���������

	// �v���C���������Ă��邩�ŃQ�[���̑��s�𔻒�
	if (player->dead() != Player::DeadReason::ALIVE)
	{
		info.status = Stage::Status::Failed;
		info.dead_reason = player->dead();
	}

	// �v���C�����S�[�����Ă��邩
	if (goal->is_completed())
	{
		info.status = Stage::Status::Clear;
	}

	return info;
}

void Stage::multi_texture_loader(std::map<std::string, const char *> _textures)
{
	std::map<std::string, LPDIRECT3DTEXTURE9>().swap(textures); // clear
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
			sprintf_s(texture_file, "%s%s", TEXTURES_DIR, record[1].c_str());
			if (FAILED(D3DXCreateTextureFromFile(d3d_device, texture_file, &textures[record[0]])))
			{
#ifdef _DEBUG
				printf("Failed to load texture file: %s\n", record[1].c_str());
#endif
			}
		}
	}

}

bool Stage::stagefile_loader(const char * filepath)
{
	// TODO: �X�e�[�W�t�@�C������̃��[�_�����
	
	std::vector<std::vector<std::string>> table;

	if (!(csv_loader(filepath, table))) return false; // csv�̓ǂݍ��݂Ɏ��s
	if (table[0].size() != 4) return false; // ��s�ڂɂ�w,h�ɂ��Ă����ė~�����̂�

	// ���ׂĖ��ɂ���@��̂��Ƃ͍l���ĂȂ��@���݂܂���
	std::map<SquarePolygonBase::PolygonTypes, polygon_vec>().swap(polygons);
	
	// �}�b�v�̃T�C�Y����ꂽ��
	map_size = POLSIZE(static_cast<float>(std::atof(table[0][0].c_str()) * CELL_WIDTH), static_cast<float>(std::atof(table[0][1].c_str()) * CELL_HEIGHT));
	
	// �w�i�̓o�^
	(background = emplace_polygon_back(SquarePolygonBase::PolygonTypes::BACKGROUND, new StageBackground(textures["BACKGROUND"], camera)))->on();

	// �v���C���̓o�^
	// �v���C���͐�ɓo�^���Ă����Ȃ��ƌ�X���邢�ł�
	(player = emplace_polygon_back(SquarePolygonBase::PolygonTypes::PLAYER, REGISTER_PLAYER(std::atof(table[0][2].c_str()) * CELL_WIDTH - CELL_WIDTH / 2, std::atof(table[0][3].c_str()) * CELL_HEIGHT - CELL_HEIGHT / 2, textures["PLAYER"], camera, polygons)))->on();

	// tmps
	AirCannon* aircannon;

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
				break;
			case 2:
				// �G
				enemies.emplace_back(emplace_polygon_back(SquarePolygonBase::PolygonTypes::ENEMY, REGISTER_ENEMY_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["ENEMY_01"], camera, polygons)));
				enemies.back()->on();
				break;
			case 3:
				// �S�[��
				(goal = emplace_polygon_back(SquarePolygonBase::PolygonTypes::GOAL, REGISTER_GOAL(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["GOAL_01"], camera, player)))->on();
				break;
			case 4:
				// �Ǐ�
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["SAMPLE1"], camera))->on();
				break;
			case 5:
				// �n�[�t���i��j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.75, textures["FLOOR_01"], camera, player))->on();
				break;
			case 6:
				// �n�[�t���i���j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.25, textures["FLOOR_01"], camera, player))->on();
				break;
			case 7:
				// switch�ŏo������Ȃɂ�
				break;
			case 8:
				// �����鏰
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_RAGGED_FLOOR(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["FLOOR_01"], camera, player))->on();
				break;
			case 9:
				// �n�[�t�ǁi�E�j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_HALF_WALL(j * CELL_WIDTH + CELL_WIDTH / 0.75, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["FLOOR_01"], camera, player))->on();
				break;
			case 10:
				// �n�[�t�ǁi���j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_HALF_WALL(j * CELL_WIDTH + CELL_WIDTH / 0.25, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["FLOOR_01"], camera, player))->on();
				break;
			case 11:
				// �g�Q�i���j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::THORN, REGISTER_THORN_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_DOWN"], camera))->on();
				break;
			case 12:
				// �g�Q�i��j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::THORN, REGISTER_THORN_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_UP"], camera))->on();
				break;
			case 13:
				// �g�Q�i�E�j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::THORN, REGISTER_THORN_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_LEFT"], camera))->on();
				break;
			case 14:
				// �g�Q�i���j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::THORN, REGISTER_THORN_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_RIGHT"], camera))->on();
				break;
			case 15:
				// �g�Q�i���j�ǁi��j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.75, textures["FLOOR_01"], camera, player))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::THORN, REGISTER_THORN_DOWN_HALF(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.25, textures["THORN_DOWN"], camera))->on();
				break;
			case 16:
				// �g�Q�i��j�ǁi���j
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::THORN, REGISTER_THORN_UP_HALF(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.75, textures["THORN_UP"], camera))->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, REGISTER_HALF_BLOCK(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.25, textures["FLOOR_01"], camera, player))->on();
				break;
			case 17:
				// �g�Q�i������j�v���C���Ɍ�������
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::THORN, REGISTER_THORN_FALL_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT / 2, textures["THORN_DOWN"], camera))->on();
				break;
			case 21:
				// ��C�C�i��j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 22:
				// ��C�C�i���j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 23:
				// ��C�C�i�E�j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 24:
				// ��C�C�i���j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::KNOCKBACK_BULLET, aircannon->get_bullet());
				break;
			case 31:
				// ���Ă�I�u�W�F�N�g
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::ITEM, REGISTER_ITEM(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["ITEM_01"], camera, polygons))->on();
				break;
			case 32:
				// �X�C�b�`
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::GIMMICK_SWITCH, REGISTER_GIMMICK_SWITCH(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["SWITCH_01"], camera, switch_sample))->on();
				break;
			case 33:
				// �}�O�}
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::MAGMA, REGISTER_MAGMA(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["MAGMA_01"], camera))->on();
				break;
			case 41:
				// �L���i��j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_UP(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			case 42:
				// �L���i���j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_DOWN(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			case 43:
				// �L���i�E�j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_RIGHT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			case 44:
				// �L���C�i���j
				aircannon = emplace_polygon_back(SquarePolygonBase::PolygonTypes::AIRCANNON, REGISTER_AIRCANNON_LEFT(j * CELL_WIDTH + CELL_WIDTH / 2, map_size.h - i * CELL_HEIGHT + CELL_HEIGHT * 0.5, textures["AIRCANNON_01"], textures["BULLET_01"], camera));
				aircannon->on();
				emplace_polygon_back(SquarePolygonBase::PolygonTypes::BULLET, aircannon->get_bullet());
				break;
			}
		}
	}

	// �g�Q�̏����Z�b�g���Ă����܂�
	std::vector<SquarePolygonBase*> floors;
	for (const auto& type : { SquarePolygonBase::PolygonTypes::SCALABLE_OBJECT, SquarePolygonBase::PolygonTypes::ENEMY, SquarePolygonBase::PolygonTypes::RAGGED_FLOOR}) floors.insert(floors.end(), polygons[type].begin(), polygons[type].end());
	for (const auto& thorn : polygons[SquarePolygonBase::PolygonTypes::THORN]) static_cast<Thorn*>(thorn)->set_floor(floors);

	// �Q�[�W�̃Z�b�g
	(gage = emplace_polygon_back(SquarePolygonBase::PolygonTypes::GAGE, new Gage(200, 200, textures["GAGE_01"])))->on();

#ifdef _DEBUG

	// ���_
	polygons[SquarePolygonBase::PolygonTypes::DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 20, 20, textures["ORIGIN"], 2, camera));
	polygons[SquarePolygonBase::PolygonTypes::DEBUG_GUIDE].back()->on();

	// x���K�C�h���C��
	polygons[SquarePolygonBase::PolygonTypes::DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 100000, 10, textures["BLOCK2"], 3, camera));
	polygons[SquarePolygonBase::PolygonTypes::DEBUG_GUIDE].back()->on();

	// y���K�C�h���C��
	polygons[SquarePolygonBase::PolygonTypes::DEBUG_GUIDE].push_back(new PlainSquarePolygon(0, 0, 10, 100000, textures["BLOCK2"], 3, camera));
	polygons[SquarePolygonBase::PolygonTypes::DEBUG_GUIDE].back()->on();

#endif
	return true;
}

// ������, �R���X�g���N�^����Ă�
void Stage::init()
{
#ifdef _DEBUG
	const auto exec_start = std::chrono::system_clock::now();
#endif
	char filepath[256]; // �t�@�C���p�X�i�[

	sprintf_s(filepath, STAGEFILES_DIR "textures_%02d.csv", info.stage_number);
	multi_texture_loader(filepath); // �e�N�X�`���̃��X�g��ǂݍ��� => ��������

	sprintf_s(filepath, STAGEFILES_DIR "stage_%02d.csv", info.stage_number);
	auto exec_result = stagefile_loader(filepath); // �|���S���t�@�C���p�X���w�肵�ēǂݍ���

	zoom_level = { 1, 1 };
	zoom_sign = Stage::Sign::ZERO;

#ifdef _DEBUG
	std::cout << "Stage Load Time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - exec_start).count() << std::endl;
#endif
	if (exec_result) info.status = Stage::Status::Ready;
	else info.status = Stage::Status::LoadError;
}

// �X�V����
void Stage::update()
{
	// ���Ԃ��C�ɂ��Ȃ�����

	// �^�C�g���ɖ߂�i���m�F�j
	if (GetKeyboardTrigger(DIK_F5))
	{
		info.status = Stage::Status::Retire;
		return;
	}

	// �^�C�g���ɖ߂�i���m�F�j
	if (GetKeyboardTrigger(DIK_F2))
	{
		printf("Camera: (%f, %f) Player: (%f, %f)\n", camera.x, camera.y, player->get_coords().x, player->get_coords().y);
	}

	// �g�k
	if (zoom_sign == Stage::Sign::ZERO && gage->can_consume())
	{
		// �ŏ��ɂ��� => �����͑傫���Ȃ�
		if (zoom_level.w > 0.5f && GetKeyboardTrigger(DIK_NUMPAD1))
		{
			zoom_level_target.w = zoom_level_target.h = 0.5f;
			zoom_sign = Stage::Sign::MINUS;
			player->lock();
			gage->consume();
		}

		// �ʏ���
		if (zoom_level.w != 1 && GetKeyboardTrigger(DIK_NUMPAD2))
		{
			zoom_level_target.w = zoom_level_target.h = 1.0f;
			zoom_sign = (zoom_level.w < 0 ? Stage::Sign::PLUS : Stage::Sign::MINUS);
			player->lock();
			gage->consume();
		}

		// �ő剻 => �����͏������Ȃ�
		if (zoom_level.w < 2.0f && GetKeyboardTrigger(DIK_NUMPAD3))
		{
			zoom_level_target.w = zoom_level_target.h = 2.0f;
			zoom_sign = Stage::Sign::PLUS;
			player->lock();
			gage->consume();
		}

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_O) && zoom_level.h < 2.0f) // �g��
		{
			zoom_sign = Stage::Sign::PLUS;
			zoom_level_target.w = zoom_level.w * 2;
			zoom_level_target.h = zoom_level.h * 2;
			player->lock();
		}
		if (GetKeyboardTrigger(DIK_L) && zoom_level.w > 0.5f) // �k��
		{
			zoom_sign = Stage::Sign::MINUS;
			zoom_level_target.w = zoom_level.w / 2;
			zoom_level_target.h = zoom_level.h / 2;
			player->lock();
		}
#endif
	}
	
	// �v���C�����W�����v������
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		player->jump();
	}

	// �v���C���ɒ͂܂����肷��
	if (GetKeyboardTrigger(DIK_U))
	{
		if (player->is_holding_item()) player->release_item();
		else player->catch_item();
	}

	// �Q�[�W�̏�����e�X�g
	if (GetKeyboardTrigger(DIK_J))
	{
		gage->consume();
	}


	// ���Ԃ��C�ɂ������
	auto current = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_update).count() < UPDATE_INTERVAL) return;
	latest_update = current;

	if (zoom_sign == Stage::Sign::PLUS)
	{
		if (zoom_level.w < zoom_level_target.w)
		{
			zoom_level.w *= 1.01f;
			zoom_level.h *= 1.01f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = Stage::Sign::ZERO;
			player->unlock();
		}
	}

	if (zoom_sign == Stage::Sign::MINUS)
	{
		if (zoom_level.w > zoom_level_target.w)
		{
			zoom_level.w /= 1.01f;
			zoom_level.h /= 1.01f;
		}
		else
		{
			zoom_level = zoom_level_target;
			zoom_sign = Stage::Sign::ZERO;
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
	auto current = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(current - latest_draw).count() < 1000 / FRAME_RATES) return;
	latest_draw = current;

	// ��������`�揈��

	// �\�[�g
	polygon_vec drawing_polygons;
	for (const auto& pol_vec : polygons)
		for (const auto& polygon : pol_vec.second)
			if(polygon->is_drawing())
				drawing_polygons.emplace_back(polygon);

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
template<typename _T> _T Stage::emplace_polygon_back(SquarePolygonBase::PolygonTypes type, _T polygon)
{
	polygons[type].emplace_back(polygon);

	return polygon;
}
