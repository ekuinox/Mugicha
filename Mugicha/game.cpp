#include "game.h"

/* �Q�[���{�� */
Game::Game()
{
	scene = scene::TITLE;
	loops = 0;
	latest_update = timeGetTime();
	latest_draw = timeGetTime();
	init();
	timeBeginPeriod(1);
}

Game::~Game()
{
	timeEndPeriod(1);
}

void Game::init()
{
	stage = new Stage();
	// �V�[���؂�ւ�
	switch_scene(scene::TITLE);
}

// ���C���̃��[�v�p�̏����C60fps�ŉ�ʕ`�������
void Game::exec()
{
	update();
	draw();
}

// �V�[���̐؂�ւ�
void Game::switch_scene(enum scene _scene)
{
	scene = _scene;
}

// ���C���̃A�b�v�f�[�g�����i�����������j
void Game::update()
{
	UpdateInput();

	auto current = timeGetTime();
	if (current - latest_update < 1) return;

	// �^�C�g�����
	if (scene == scene::TITLE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch_scene(scene::MAIN);
			latest_update = current;
		}
	}

	// ���C��
	if (scene == scene::MAIN)
	{
		main();
		latest_update = current;
	}

	// �Q�[���I�[�o
	if (scene == scene::GAMEOVER)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_update = current;
		}
	}

	// �Q�[���N���A
	if (scene == scene::GAMECLEAR)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			init();
			switch_scene(scene::MAIN);
			latest_update = current;
		}
	}
}

// �|���S�����܂Ƃ߂ăh���[
void Game::draw()
{
	auto current = timeGetTime();
	if (current - latest_draw < 1000 / 60) return;
	latest_draw = current;
}


// �Q�[���̈�ԃ��C��
bool Game::main(bool reset)
{
	stage->exec();

	return true;
}