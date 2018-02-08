#include "game.h"

/* �Q�[���{�� */
Game::Game()
{
	scene = scene::TITLE;
	loops = 0;
	exec_latest = timeGetTime();
	init();
	timeBeginPeriod(1);
}

Game::~Game()
{
	// �e�N�X�`���N���A
	for (auto tex : textures)
	{
		if (tex.second)
		{
			tex.second->Release();
			tex.second = NULL;
		}
	}
	timeEndPeriod(1);
}

void Game::init()
{
	if (textures.empty())
	{
		multi_texture_loader({
//			{ "__TAG__", "__FILE_PATH__" },
			{ "BACKGROUND", "./resources/textures/background.jpg" },
			{ "PLAYER", "./resources/textures/player.jpg" },
		});
	}
	
	std::map<std::string, polygon_vec>().swap(polygons); // ���ׂĖ��ɂ���@��̂��Ƃ͍l���ĂȂ��@���݂܂���

	// �w�i�̓o�^
	polygons["BACKGROUNDS"].push_back(new Background(textures["BACKGROUND"]));
	polygons["BACKGROUNDS"][0]->switch_status(true);

	// �v���C���̓o�^
	polygons["PLAYERS"].push_back(new Player(textures["PLAYER"]));
	polygons["PLAYERS"][0]->switch_status(false);

	// �V�[���؂�ւ�
	switch_scene(scene::TITLE);
}

// ���C���̃��[�v�p�̏����C60fps�ŉ�ʕ`�������
void Game::exec()
{
	auto current = timeGetTime();
	update();
	if (1000 / 60 < current - exec_latest)
	{
		draw();
		exec_latest = current;
	}
}

// �V�[���̐؂�ւ�
void Game::switch_scene(enum scene _scene)
{
	scene = _scene;
	for (const auto& _polygons : polygons)
	{
		for(const auto& polygon : _polygons.second) polygon->switch_drawing(false); // ��x���ׂẴ|���S�����\���ɂ���
	}

	std::vector<std::string> labels; // �h���[����|���S���̃��x�����l�߂Ă���

	// �\������|���S�����V�[���ŐU�蕪���邯�ǁC�悭�Ȃ�
	// �Ȃɂ��ǂ��Ȃ������ď�Ƀ|���S���͂����āC���̃V�[���ł͕K�v�̂Ȃ��|���S���������Ă�
	// �Ȃ̂ł��̐�C�����ꂽ��
	switch (scene)
	{
	case scene::TITLE:
		labels = {
			"BACKGROUNDS"
		};
		break;
	case scene::MAIN:
		labels = {
			"BACKGROUNDS",
			"PLAYERS",
		};
		break;
	case scene::GAMEOVER:
		labels = {
			"BACKGROUNDS"
		};
		break;
	case scene::GAMECLEAR:
		labels = {
			"BACKGROUNDS"
		};
		break;
	case scene::END:
		break;
	}

	for (const auto& label : labels)
	{
		for(const auto& polygon : polygons[label]) polygon->switch_drawing(true); // ���x���Ŏw�肳�ꂽ�|���S����\���ɂ��Ă���
	}
}

// ���C���̃A�b�v�f�[�g�����i�����������j
void Game::update()
{
	UpdateInput();

	DWORD current = timeGetTime();
	static DWORD latest_scene_update;

	// �^�C�g�����
	if (scene == scene::TITLE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(scene::MAIN);
			latest_scene_update = current;
		}
	}

	// ���C��
	if (scene == scene::MAIN)
	{
		// ���C���Ŏg���|���S���̗L����
		polygons["PLAYERS"][0]->switch_status(TRUE);

		main();
		latest_scene_update = current;
	}

	// �Q�[���I�[�o
	if (scene == scene::GAMEOVER)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_scene_update = current;
		}
	}

	// �Q�[���N���A
	if (scene == scene::GAMECLEAR)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_scene_update = current;
		}
	}

	// �|���S���̑S�X�V
	for (const auto& _polygons : polygons)
	{
		for (const auto& polygon : _polygons.second) polygon->update();
	}
}

// �|���S�����܂Ƃ߂ăh���[
void Game::draw()
{
	d3d_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(d3d_device->BeginScene()))
	{
		for (const auto& _polygons : polygons)
		{
			// �`�揇���Ǘ��ł���悤�ɂ���K�v�������ꗈ��
			for (const auto& polygon : _polygons.second) polygon->draw();
		}
	}
	d3d_device->EndScene();

	d3d_device->Present(NULL, NULL, NULL, NULL);
}


// �Q�[���̈�ԃ��C��
bool Game::main(bool reset)
{
	DWORD current = timeGetTime();

	// ����
	if (current - latest_update > 1) // 1ms�Ԋu��
	{ 
		
	}

	return true;
}

void Game::multi_texture_loader(std::map<std::string, std::string> _textures)
{
	for (auto _texture : _textures)
	{
		D3DXCreateTextureFromFile(d3d_device, _texture.second.c_str(), &textures[_texture.first]); // ��ēǂݍ���
	}
}